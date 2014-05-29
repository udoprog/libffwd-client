#include "ffwd.hpp"
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <vector>

const ffwd::client c;

namespace po = boost::program_options;

template<typename T> T add_tags(T d, po::variables_map& vm) {
    if (!vm.count("tag"))
        return d;

    std::vector<std::string> tags = vm["tag"].as<std::vector<std::string> >();

    BOOST_FOREACH(std::string tag, tags) {
        d = d.tag(tag);
    }

    return d;
}

template<typename T> T add_attributes(T d, po::variables_map& vm) {
    if (!vm.count("attribute")) {
        return d;
    }

    std::vector<std::string> attributes = vm["attribute"].as<std::vector<std::string> >();

    BOOST_FOREACH(std::string a, attributes) {
        const size_t n = a.find("=");

        if (n == std::string::npos)
            continue;

        const std::string key = a.substr(0, n);
        const std::string value = a.substr(n + 1, a.size());

        d = d.attribute(key, value);
    }

    return d;
}

ffwd::event build_event(po::variables_map& vm) {
    ffwd::event e;

    if (vm.count("time"))
        e = e.time(vm["time"].as<uint64_t>());

    if (vm.count("key"))
        e = e.key(vm["key"].as<std::string>());

    if (vm.count("host"))
        e = e.host(vm["host"].as<std::string>());

    if (vm.count("value"))
        e = e.value(vm["value"].as<double>());

    if (vm.count("description"))
        e = e.description(vm["description"].as<std::string>());

    if (vm.count("state"))
        e = e.state(vm["state"].as<std::string>());

    if (vm.count("ttl"))
        e = e.ttl(vm["ttl"].as<int64_t>());

    e = add_tags(e, vm);
    e = add_attributes(e, vm);
    return e;
}

ffwd::metric build_metric(po::variables_map& vm) {
    ffwd::metric m;

    if (vm.count("time"))
        m = m.time(vm["time"].as<uint64_t>());

    if (vm.count("key"))
        m = m.key(vm["key"].as<std::string>());

    if (vm.count("host"))
        m = m.host(vm["host"].as<std::string>());

    if (vm.count("value"))
        m = m.value(vm["value"].as<double>());

    m = add_tags(m, vm);
    m = add_attributes(m, vm);
    return m;
}

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("event,e", "produce event")
        ("metric,m", "produce metric")
        ("proc", po::value<std::string>(), "set proc (metric only)")
        ("time", po::value<uint64_t>(), "set time in ms")
        ("key,k", po::value<std::string>(), "set key")
        ("host,H", po::value<std::string>(), "set host")
        ("value,v", po::value<double>(), "set value")
        ("description,d", po::value<std::string>(), "set description (event only)")
        ("state", po::value<std::string>(), "set state (event only)")
        ("ttl", po::value<int64_t>(), "set time-to-live (event only)")
        ("tag", po::value<std::vector<std::string> >(), "add tag")
        ("attribute,a", po::value<std::vector<std::string> >(), "add attribute")
    ;

    po::variables_map vm;

    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (vm.count("help")) {
        std::cout << "Usage: " << argv[0] << " <-e|-m> <opts>" << std::endl
                  << std::endl
                  << desc << std::endl;
        return 1;
    }

    if (vm.count("event")) {
        ffwd::event e = build_event(vm);
        c.send(e);
        std::cout << "sent: event" << std::endl;
    }

    if (vm.count("metric")) {
        ffwd::metric m = build_metric(vm);
        c.send(m);
        std::cout << "sent: metric" << std::endl;
    }

    if (!c.ready()) {
        std::cerr << "Not ready to send" << std::endl;
        return 1;
    }

    return 0;
}
