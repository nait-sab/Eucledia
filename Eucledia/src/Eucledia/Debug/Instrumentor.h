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
        Instrumentor() : _currentSession(nullptr) {}

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

            std::string name = result.name;
            std::replace(name.begin(), name.end(), '"', '\'');

            json << ",{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << (result.elapsedTime.count()) << ',';
            json << "\"name\":\"" << name << "\",";
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
}

#define EUCLEDIA_PROFILE 1

#if EUCLEDIA_PROFILE
    #define EUCLEDIA_PROFILE_BEGIN_SESSION(name, filepath) ::Eucledia::Instrumentor::get().beginSession(name, filepath)
    #define EUCLEDIA_PROFILE_END_SESSION() ::Eucledia::Instrumentor::get().endSession()
    #define EUCLEDIA_PROFILE_SCOPE(name) ::Eucledia::InstrumentationTimer timer##__LINE__(name);
    #define EUCLEDIA_PROFILE_FUNCTION() EUCLEDIA_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define EUCLEDIA_PROFILE_BEGIN_SESSION(name, filepath)
    #define EUCLEDIA_PROFILE_END_SESSION()
    #define EUCLEDIA_PROFILE_SCOPE(name)
    #define EUCLEDIA_PROFILE_FUNCTION()
#endif