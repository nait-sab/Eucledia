#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>

namespace Eucledia
{
    using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

    struct ProfileResult
    {
        std::string name;
        FloatingPointMicroseconds start;
        std::chrono::microseconds elapsedTime;
        std::thread::id threadID;
    };

    struct InstrumentationSession
    {
        std::string name;
    };

    class Instrumentor
    {
    public:
        Instrumentor(const Instrumentor&) = delete;
        Instrumentor(Instrumentor&&) = delete;

        void beginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            std::lock_guard lock(_mutex);

            if (_currentSession)
            {
                if (Log::getCoreLogger)
                {
                    EUCLEDIA_CORE_ERROR("Instrumentor::beginSession('{0}') when session '{1}' already open", name, _currentSession->name);
                }

                internalEndSession();
            }

            _outputStream.open(filepath);
            
            if (_outputStream.is_open())
            {
                _currentSession = new InstrumentationSession({ name });
                writeHeader();
            }
            else
            {
                if (Log::getCoreLogger())
                {
                    EUCLEDIA_CORE_ERROR("Instrumentor::beginSession could not open results file '{0}'", filepath);
                }
            }
        }

        void endSession()
        {
            std::lock_guard lock(_mutex);
            internalEndSession();
        }

        void writeProfile(const ProfileResult& result)
        {
            std::stringstream json;

            json << ",{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << (result.elapsedTime.count()) << ',';
            json << "\"name\":\"" << result.name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.threadID << ",";
            json << "\"ts\":" << result.start.count();
            json << "}";

            std::lock_guard lock(_mutex);

            if (_currentSession)
            {
                _outputStream << json.str();
                _outputStream.flush();
            }
        }

        static Instrumentor& get()
        {
            static Instrumentor instance;
            return instance;
        }

    private:
        Instrumentor() : _currentSession(nullptr) {}

        ~Instrumentor()
        {
            endSession();
        }

        std::mutex _mutex;
        InstrumentationSession* _currentSession;
        std::ofstream _outputStream;
        int _profileCount;

        void writeHeader()
        {
            _outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
            _outputStream.flush();
        }

        void writeFooter()
        {
            _outputStream << "]}";
            _outputStream.flush();
        }

        void internalEndSession()
        {
            if (_currentSession)
            {
                writeFooter();
                _outputStream.close();
                delete _currentSession;
                _currentSession = nullptr;
            }
        }
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name) : _name(name), _stopped(false)
        {
            _startTimepoint = std::chrono::steady_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!_stopped)
            {
                stop();
            }
        }

        void stop()
        {
            auto endTimepoint = std::chrono::steady_clock::now();
            auto highRestart = FloatingPointMicroseconds{ _startTimepoint.time_since_epoch() };
            auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() -
                std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch();

            Instrumentor::get().writeProfile({ _name, highRestart, elapsedTime, std::this_thread::get_id() });

            _stopped = true;
        }
    private:
        const char* _name;
        std::chrono::time_point<std::chrono::steady_clock> _startTimepoint;
        bool _stopped;
    };

    namespace InstrumentorUtils 
    {
        template <size_t N>
        struct ChangeResult
        {
            char data[N];
        };


        template <size_t N, size_t K>
        constexpr auto cleanupOutputString(const char(&expr)[N], const char(&remove)[K])
        {
            ChangeResult<N> result = {};

            size_t srcIndex = 0;
            size_t dstIndex = 0;

            while (srcIndex < N)
            {
                size_t matchIndex = 0;

                while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
                {
                    matchIndex++;
                }

                if (matchIndex == K - 1)
                {
                    srcIndex += matchIndex;
                }

                result.data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
                srcIndex++;
            }

            return result;
        }
    }
}

#define EUCLEDIA_PROFILE 0

#if EUCLEDIA_PROFILE
    #define EUCLEDIA_PROFILE_BEGIN_SESSION(name, filepath) ::Eucledia::Instrumentor::get().beginSession(name, filepath)
    #define EUCLEDIA_PROFILE_END_SESSION() ::Eucledia::Instrumentor::get().endSession()
    #define EUCLEDIA_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName = ::Eucledia::InstrumentorUtils::cleanupOutputString(name, "__cdecl"); timer##__LINE__(fixedName##line.data);
    #define EUCLEDIA_PROFILE_SCOPE_LINE(name, line) EUCLEDIA_PROFILE_SCOPE_LINE2(name, line)
    #define EUCLEDIA_PROFILE_SCOPE(name) EUCLEDIA_PROFILE_SCOPE_LINE(name, __LINE__)
    #define EUCLEDIA_PROFILE_FUNCTION() EUCLEDIA_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define EUCLEDIA_PROFILE_BEGIN_SESSION(name, filepath)
    #define EUCLEDIA_PROFILE_END_SESSION()
    #define EUCLEDIA_PROFILE_SCOPE_LINE2(name, line)
    #define EUCLEDIA_PROFILE_SCOPE_LINE(name, line)
    #define EUCLEDIA_PROFILE_SCOPE(name)
    #define EUCLEDIA_PROFILE_FUNCTION()
#endif