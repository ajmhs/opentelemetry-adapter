/*
* (c) Copyright, Real-Time Innovations, 2024.  All rights reserved.
* RTI grants Licensee a license to use, modify, compile, and create derivative
* works of the software solely for use with RTI Connext DDS. Licensee may
* redistribute copies of the software provided that all such copies are subject
* to this license. The software is provided "as is", with no warranty of any
* type, including any warranty for fitness for any purpose. RTI is under no
* obligation to maintain or support the software. RTI shall not be liable for
* any incidental or consequential damages arising out of the use or inability
* to use the software.
*/

#include <iostream>
#include <vector>
#include <climits>
#include <functional>
#include <random>
#include <chrono>
#include <thread>
#include <dds/pub/ddspub.hpp>
#include <rti/config/Logger.hpp>  // for logging
#include "application.hpp"  // for command line parsing and ctrl-c
#include "telemetry.hpp"

static std::mt19937 twister;
static std::uniform_real_distribution<double> distribution(1, 999);

namespace MetricTypes {
    enum Enum {
        UInt64Counter = 0,
        DoubleCounter = 1,
        UInt64Histogram = 2,
        DoubleHistogram = 3,
        UInt64UpDownCounter = 4,
        DoubleUpDownCounter = 5    
    };
}

void PublishUint64Counter(dds::pub::DataWriter< ::Metrics::Metric> &writer, const int counter)
{
    uint64_t val = (counter * 1000) + (int)distribution(twister);

    ::Metrics::Metric metric;
    metric.name("rti.example.uint64.counter");
    metric.description("Example UInt64 Counter");
    metric.unit("Jiffy");
    metric.data()._d(MetricTypes::UInt64Counter);
    metric.data().uint64_counter().value(val);
    writer.write(metric);

    std::cout << "Wrote: " << metric << std::endl;
}

void PublishDoubleCounter(dds::pub::DataWriter< ::Metrics::Metric> &writer, const int counter)
{
    double val = (distribution(twister) / 1000) + (double)counter;

    ::Metrics::Metric metric;
    metric.name("rti.example.double.counter");
    metric.description("Example Double Counter");
    metric.unit("Micro-Fortnight");
    metric.data()._d(MetricTypes::DoubleCounter);
    metric.data().double_counter().value(val);
    writer.write(metric);

    std::cout << "Wrote: " << metric << std::endl;
}

void PublishUInt64Histogram(dds::pub::DataWriter< ::Metrics::Metric> &writer, const int counter) {

    ::Metrics::Metric metric;
    metric.name("rti.example.uint64.histogram");
    metric.description("Example UInt64 Histogram");
    metric.unit("Milliblatt");
    metric.data()._d(MetricTypes::UInt64Histogram);
    
    for (uint32_t i = 0; i < 5; ++i) {
        ::Metrics::Label label;
        label.key("key" + std::to_string(i));
        label.value("value" + std::to_string(i));
        metric.data().uint64_histogram().labels().push_back(label);
    }

    for (uint32_t i = 0; i < 20; ++i) {
        metric.data().uint64_histogram().buckets().push_back(counter + (int)distribution(twister));
    }

    writer.write(metric);
    std::cout << "Wrote: " << metric << std::endl;
}
        
void PublishDoubleHistogram(dds::pub::DataWriter< ::Metrics::Metric> &writer, const int counter) {

    ::Metrics::Metric metric;
    metric.name("rti.example.double.histogram");
    metric.description("Example Double Histogram");
    metric.unit("Light-nanosecond");
    metric.data()._d(MetricTypes::DoubleHistogram);
    
    for (uint32_t i = 0; i < 5; ++i) {
        ::Metrics::Label label;
        label.key("key" + std::to_string(i));
        label.value("value" + std::to_string(i));
        metric.data().double_histogram().labels().push_back(label);
    }

    for (uint32_t i = 0; i < 20; ++i) {
        metric.data().double_histogram().buckets().push_back((distribution(twister) / 1000) + (double)counter);
    }

    writer.write(metric);
    std::cout << "Wrote: " << metric << std::endl;
}

void PublishUInt64UpDownCounter(dds::pub::DataWriter< ::Metrics::Metric> &writer, const int counter) {
    
    ::Metrics::Metric metric;
    metric.name("rti.example.uint64.updowncounter");
    metric.description("Example UInt64 UpDownCounter");
    metric.unit("Sheppey");    
    metric.data()._d(MetricTypes::UInt64UpDownCounter);
    metric.data().uint64_updown_counter().value(INT_MAX - counter);
    writer.write(metric);
    
    std::cout << "Wrote: " << metric << std::endl;
}

void PublishDoubleUpDownCounter(dds::pub::DataWriter< ::Metrics::Metric> &writer, const int counter) {
    
    ::Metrics::Metric metric;
    metric.name("rti.example.double.updowncounter");
    metric.description("Example Double UpDownCounter");
    metric.unit("Muggeseggele");
    metric.data()._d(MetricTypes::DoubleUpDownCounter);
    metric.data().double_updown_counter().value(INT_MAX - (distribution(twister) / 1000));
    writer.write(metric);
    
    std::cout << "Wrote: " << metric << std::endl;
}

void run_publisher_application(unsigned int domain_id, const std::string& qos_file, unsigned int sample_count)
{
    dds::core::QosProvider qos_provider(qos_file);

    // Start communicating in a domain, usually one participant per application
    dds::domain::DomainParticipant participant(
            domain_id,
            qos_provider.participant_qos());

    // Create a Topic with a name and a datatype
    dds::topic::Topic< ::Metrics::Metric> topic(participant, "Application Telemetry");

    // Create a Publisher
    dds::pub::Publisher publisher(participant);

    // Create a DataWriter with default QoS
    dds::pub::DataWriter< ::Metrics::Metric> writer(
	    publisher, 
	    topic,
        qos_provider.datawriter_qos("telemetry_library::telemetry_profile"));

    std::vector<std::function<void(dds::pub::DataWriter< ::Metrics::Metric>&, const int)>> functions = {
        PublishUint64Counter,
        PublishDoubleCounter,
        PublishUInt64Histogram,
        PublishDoubleHistogram,
        PublishUInt64UpDownCounter,
        PublishDoubleUpDownCounter
    };

    // Main loop, publish telemetry data    
    for (unsigned int samples_written = 0, idx = 0;
        !application::shutdown_requested && samples_written < sample_count;
        ++samples_written, idx = (idx + 1) % functions.size()) {
        
        functions[idx](writer, samples_written);        

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char *argv[])
{
    using namespace application;

    // Parse arguments and handle control-C
    auto arguments = parse_arguments(argc, argv);
    if (arguments.parse_result == ParseReturn::exit) {
        return EXIT_SUCCESS;
    } else if (arguments.parse_result == ParseReturn::failure) {
        return EXIT_FAILURE;
    }
    setup_signal_handlers();

    // Sets Connext verbosity to help debugging
    rti::config::Logger::instance().verbosity(arguments.verbosity);

    // Initialize random seed
    std::random_device rd;
    unsigned seed = rd.entropy() ? rd() : static_cast<unsigned>(
        std::chrono::system_clock::now().time_since_epoch().count());
    twister.seed(seed);    

    try {
        run_publisher_application(arguments.domain_id, arguments.qos_file, arguments.sample_count);
    } catch (const std::exception& ex) {
        // This will catch DDS exceptions
        std::cerr << "Exception in run_publisher_application(): " << ex.what()
        << std::endl;
        return EXIT_FAILURE;
    }

    // Releases the memory used by the participant factory.  Optional at
    // application exit
    dds::domain::DomainParticipant::finalize_participant_factory();

    return EXIT_SUCCESS;
}