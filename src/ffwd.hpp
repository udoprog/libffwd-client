#ifndef _FFWD_HPP_
#define _FFWD_HPP_

#include <sys/socket.h>
#include <string>
#include <stdint.h>

#include <list>

namespace ffwd {
    typedef std::pair<std::string, std::string> attribute_pair;

    template <typename S> class base_type {
    protected:
        base_type(S s) : fields_(s) { }
        inline S set(S f) const { return fields_ | f; };
        inline bool has(S f) const { return (fields_ & f) != 0; };
    private:
        S fields_;
    };

    class event : public base_type<uint32_t> {
    public:
        static const int TIME        = 1 << 0;
        static const int KEY         = 1 << 1;
        static const int VALUE       = 1 << 2;
        static const int HOST        = 1 << 3;
        static const int STATE       = 1 << 4;
        static const int DESCRIPTION = 1 << 5;
        static const int TTL         = 1 << 6;
        static const int TAGS        = 1 << 7;
        static const int ATTRIBUTES  = 1 << 8;

        event();
        event(const std::string key);
        event(
            const uint32_t has,
            const uint64_t time,
            const std::string key,
            const double value,
            const std::string host,
            const std::string state,
            const std::string description,
            const int64_t ttl,
            const std::list<std::string> tags,
            const std::list<attribute_pair> attr
        );

        std::string serialize() const;

        const event time(uint64_t time) const;
        const event key(const std::string key) const;
        const event value(double value) const;
        const event host(const std::string host) const;
        const event state(const std::string state) const;
        const event description(const std::string description) const;
        const event ttl(int64_t ttl) const;
        const event tag(const std::string tag) const;
        const event attribute(const std::string key, const std::string value) const;
    private:
        uint64_t time_;
        std::string key_;
        double value_;
        std::string host_;
        std::string state_;
        std::string description_;
        int64_t ttl_;
        std::list<std::string> tags_;
        std::list<attribute_pair> attributes_;
    };

    /**
     *
     */
    class metric : public base_type<uint32_t> {
    public:
        static const uint32_t PROC       = 1 << 0;
        static const uint32_t TIME       = 1 << 1;
        static const uint32_t KEY        = 1 << 2;
        static const uint32_t VALUE      = 1 << 3;
        static const uint32_t HOST       = 1 << 4;
        static const uint32_t TAGS       = 1 << 5;
        static const uint32_t ATTRIBUTES = 1 << 6;

        metric();
        metric(const std::string key);
        metric(
            const uint32_t has,
            const std::string proc,
            const uint64_t time,
            const std::string key,
            const double value,
            const std::string host,
            const std::list<std::string> tags,
            const std::list<attribute_pair> attributes
        );

        std::string serialize() const;

        const metric proc(const std::string proc) const;
        const metric time(uint64_t time) const;
        const metric key(const std::string key) const;
        const metric value(double value) const;
        const metric host(const std::string host) const;
        const metric tag(const std::string tag) const;
        const metric attribute(const std::string key, const std::string value) const;
    private:
        std::string proc_;
        uint64_t time_;
        std::string key_;
        double value_;
        std::string host_;
        std::list<std::string> tags_;
        std::list<attribute_pair> attributes_;
    };

    class client {
    public:
        static const unsigned int BUFFER_SIZE = 2048;

        client(std::string host="localhost", std::string port="19091");
        bool ready() const;
        void send(const metric m) const;
        void send(const event e) const;
    private:
        int socket_;
        void send_frame(const std::string& string) const;
    };
}

#endif /* _FFWD_HPP_ */
