#include "ffwd.hpp"
#include "proto/protocol0.pb.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <cstdio>
#include <cstdlib>

void connect_socket(std::string host, std::string port, int* result);

namespace ffwd {
    const static uint32_t LATEST_VERSION = 0;

    template<typename T>
    inline void add_tags(T* data, std::list<std::string> values) {
        typedef std::list<std::string>::const_iterator Iter;
        for (Iter i = values.begin(); i != values.end(); ++i)
            data->add_tags(*i);
    }

    template<typename T>
    inline void add_attributes(T* data, std::list<attribute_pair> values) {
        typedef std::list<attribute_pair>::const_iterator Iter;
        for (Iter i = values.begin(); i != values.end(); ++i) {
            FFWD::Protocol0::Attribute* to = data->add_attributes();
            to->set_key(i->first);
            to->set_value(i->second);
        }
    }

    metric::metric()
    : base_type(0), proc_(), time_(), key_(), value_(), host_(),
      tags_(), attributes_() { }

    metric::metric(const std::string key)
    : base_type(0), proc_(), time_(), key_(key), value_(), host_(),
      tags_(), attributes_() { }

    metric::metric(
        const uint32_t has,
        const std::string proc,
        const uint64_t time,
        const std::string key,
        const double value,
        const std::string host,
        const std::list<std::string> tags,
        const std::list<attribute_pair> attributes
    )
    : base_type(has), proc_(proc), time_(time), key_(key), value_(value),
      host_(host), tags_(tags), attributes_(attributes) { }

    std::string metric::serialize() const {
        FFWD::Protocol0::Message message;
        FFWD::Protocol0::Metric *m = message.mutable_metric();

        if (has(PROC))
            m->set_proc(proc_);

        if (has(TIME))
            m->set_time(time_);

        if (has(KEY))
            m->set_key(key_);

        if (has(VALUE))
            m->set_value(value_);

        if (has(HOST))
            m->set_host(host_);

        if (has(TAGS))
            add_tags(m, tags_);

        if (has(ATTRIBUTES))
            add_attributes(m, attributes_);

        std::string string;
        message.SerializeToString(&string);
        return string;
    }

    const metric metric::proc(const std::string proc) const {
        return metric(set(PROC), proc, time_, key_, value_, host_, tags_, attributes_);
    }

    const metric metric::time(uint64_t time) const {
        return metric(set(TIME), proc_, time, key_, value_, host_, tags_, attributes_);
    }

    const metric metric::key(const std::string key) const {
        return metric(set(KEY), proc_, time_, key, value_, host_, tags_, attributes_);
    }

    const metric metric::value(double value) const {
        return metric(set(VALUE), proc_, time_, key_, value, host_, tags_, attributes_);
    }

    const metric metric::host(const std::string host) const {
        return metric(set(HOST), proc_, time_, key_, value_, host, tags_, attributes_);
    }

    const metric metric::tag(const std::string tag) const {
        std::list<std::string> tags(tags_);
        tags.push_back(tag);
        return metric(set(TAGS), proc_, time_, key_, value_, host_, tags, attributes_);
    }

    const metric metric::attribute(const std::string key, const std::string value) const {
        std::list<attribute_pair> attributes(attributes_);
        attributes.push_back(attribute_pair(key, value));
        return metric(set(ATTRIBUTES), proc_, time_, key_, value_, host_, tags_, attributes);
    }

    event::event()
    : base_type(0), time_(), key_(), value_(), host_(), state_(),
      description_(), ttl_(), tags_(), attributes_() { }

    event::event(const std::string key)
    : base_type(0), time_(), key_(key), value_(), host_(), state_(),
      description_(), ttl_(), tags_(), attributes_() { }

    event::event(
        const uint32_t has,
        const uint64_t time,
        const std::string key,
        const double value,
        const std::string host,
        const std::string state,
        const std::string description,
        const int64_t ttl,
        const std::list<std::string> tags,
        const std::list<attribute_pair> attributes
    )
    : base_type(has), time_(time), key_(key), value_(value), host_(host), state_(state),
      description_(description), ttl_(ttl), tags_(tags), attributes_(attributes) { }

    std::string event::serialize() const {
        FFWD::Protocol0::Message message;
        FFWD::Protocol0::Event *e = message.mutable_event();

        if (has(TIME))
            e->set_time(time_);

        if (has(KEY))
            e->set_key(key_);

        if (has(VALUE))
            e->set_value(value_);

        if (has(HOST))
            e->set_host(host_);

        if (has(STATE))
            e->set_state(state_);

        if (has(DESCRIPTION))
            e->set_description(description_);

        if (has(TTL))
            e->set_ttl(ttl_);

        if (has(TAGS))
            add_tags(e, tags_);

        if (has(ATTRIBUTES))
            add_attributes(e, attributes_);

        std::string string;
        message.SerializeToString(&string);
        return string;
    }

    const event event::time(uint64_t time) const {
        return event(set(TIME), time, key_, value_, host_, state_,
                     description_, ttl_, tags_, attributes_);
    }

    const event event::key(const std::string key) const {
        return event(set(KEY), time_, key, value_, host_, state_,
                     description_, ttl_, tags_, attributes_);
    }

    const event event::value(double value) const {
        return event(set(VALUE), time_, key_, value, host_, state_,
                     description_, ttl_, tags_, attributes_);
    }

    const event event::host(const std::string host) const {
        return event(set(HOST), time_, key_, value_, host, state_,
                     description_, ttl_, tags_, attributes_);
    }

    const event event::state(const std::string state) const {
        return event(set(STATE), time_, key_, value_, host_, state,
                     description_, ttl_, tags_, attributes_);
    }

    const event event::description(const std::string description) const {
        return event(set(DESCRIPTION), time_, key_, value_, host_, state_,
                     description, ttl_, tags_, attributes_);
    }

    const event event::ttl(int64_t ttl) const {
        return event(set(TTL), time_, key_, value_, host_, state_,
                     description_, ttl, tags_, attributes_);
    }

    const event event::tag(const std::string tag) const {
        std::list<std::string> tags(tags_);
        tags.push_back(tag);
        return event(set(TAGS), time_, key_, value_, host_, state_,
                     description_, ttl_, tags, attributes_);
    }

    const event event::attribute(const std::string key, const std::string value) const {
        std::list<attribute_pair> attributes(attributes_);
        attributes.push_back(attribute_pair(key, value));
        return event(set(ATTRIBUTES), time_, key_, value_, host_, state_,
                     description_, ttl_, tags_, attributes);
    }

    client::client(std::string host, std::string port) {
        connect_socket(host, port, &this->socket_);
    }

    bool client::ready() const {
        return this->socket_ != -1;
    }

    void client::send(const metric m) const {
        if (!ready())
            return;

        send_frame(m.serialize());
    }

    void client::send(const event e) const {
        if (!ready())
            return;

        send_frame(e.serialize());
    }

    void client::send_frame(const std::string& string) const {
        size_t length = string.size() + 8;

        // Silently drop buffers too large.
        if (length > BUFFER_SIZE)
            return;

        char buf[BUFFER_SIZE];
        char* pos = buf;

        uint32_t n_version = htonl(LATEST_VERSION);
        uint32_t n_length = htonl(length);

        memcpy(pos, &n_version, sizeof(uint32_t));
        pos += sizeof(uint32_t);
        memcpy(pos, &n_length, sizeof(uint32_t));
        pos += sizeof(uint32_t);
        memcpy(pos, string.c_str(), string.size());

        ::send(this->socket_, buf, length, 0);
    }
}

bool resolve(std::string host, std::string port, struct addrinfo* result) {
    struct addrinfo hints;
    struct addrinfo* list;
    struct addrinfo* iter;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &list) != 0)
        return false;

    for (iter = list; iter != NULL; iter = iter->ai_next)
        break;

    if (iter == NULL) {
        freeaddrinfo(list);
        return false;
    }

    *result = *iter;
    return true;
}

void connect_socket(std::string host, std::string port, int* result) {
    struct addrinfo addr;
    *result = -1;

    if (!resolve(host, port, &addr))
        return;

    int sock = socket(addr.ai_family, addr.ai_socktype, 0);

    if (sock == -1)
        return;

    if (connect(sock, addr.ai_addr, addr.ai_addrlen) == -1) {
        close(sock);
        return;
    }

    *result = sock;
}
