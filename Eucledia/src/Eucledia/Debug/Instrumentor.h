#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Eucledia
{
    struct ProfileResult
    {
        std::string name;
        long long start, end;
        uint32_t threadID;
    };

    struct InstrumentationSession
    {
        std::string name;
    };

    class Instrumentor
    {
    public:
        Instrumentor() : _currentSession(nullptr), _profileCount(0) {}

        void beginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            _outputStream.open(filepath);
            writeHeader();
            _currentSession = new InstrumentationSession{ name };
        }

        void endSession()
        {
            writeFooter();
            _outputStream.close();
            delete _currentSession;
            _currentSession = nullptr;
            _profileCount = 0;
        }

        void writeProfile(const ProfileResult& result)
        {
            if (_profileCount++ > 0)
                _outputStream << ",";

            std::string name = result.name;
            std::replace(name.begin(), name.end(), '"', '\'');

            _outputStream << "{";
            _outputStream << "\"cat\":\"function\",";
            _outputStream << "\"dur\":" << (result.end - result.start) << ',';
            _outputStream << "\"name\":\"" << name << "\",";
            _outputStream << "\"ph\":\"X\",";
            _outputStream << "\"pid\":0,";
            _outputStream << "\"tid\":" << result.threadID << ",";
            _outputStream << "\"ts\":" << result.start;
            _outputStream << "}";

            _outputStream.flush();
        }

        void writeHeader()
        {
            _outputStream << "{\"otherData\": {},\"traceEvents\":[";
            _outputStream.flush();
        }

        void writeFooter()
        {
            _outputStream << "]}";
            _outputStream.flush();
        }

        static Instrumentor& get()
        {
            static Instrumentor instance;
            return instance;
        }

    private:
        InstrumentationSession* _currentSession;
        std::ofstream _outputStream;
        int _profileCount;
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name) : _name(name), _stopped(false)
        {
            _startTimepoint = std::chrono::high_resolution_clock::now();
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
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::get().writeProfile({ _name, start, end, threadID });

            _stopped = true;
        }
    private:
        const char* _name;
        std::chrono::time_point<std::chrono::high_resolution_clock> _startTimepoint;
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