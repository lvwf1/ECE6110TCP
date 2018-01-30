#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-layout-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("TcpThroughputMeasurement");

int main(int argc, char *argv[]) {

    SeedManager::SetSeed(1); // This will ensure that all of our results are the same

    uint32_t nSpokes = 8; // The number of spokes on each star should be setup in your program to default to 8.
    std::string Protocol = "TcpHybla";

    // Process the command line

    CommandLine cmd;
    cmd.AddValue("nSpokes", "Number of spokes to place in each star", nSpokes);
    cmd.AddValue("Protocol",
                 "TCP Protocol to use: TcpNewReno, "
                         "TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, "
                         "TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus ", Protocol);
    cmd.Parse(argc, argv);

    // Select TCP variant
    if (Protocol.compare("TcpNewReno") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpNewReno::GetTypeId()));
    } else if (Protocol.compare("TcpHybla") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpHybla::GetTypeId()));
    } else if (Protocol.compare("TcpHighSpeed") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpHighSpeed::GetTypeId()));
    } else if (Protocol.compare("TcpVegas") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpVegas::GetTypeId()));
    } else if (Protocol.compare("TcpScalable") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpScalable::GetTypeId()));
    } else if (Protocol.compare("TcpHtcp") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpHtcp::GetTypeId()));
    } else if (Protocol.compare("TcpVeno") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpVeno::GetTypeId()));
    } else if (Protocol.compare("TcpBic") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpBic::GetTypeId()));
    } else if (Protocol.compare("TcpYeah") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpYeah::GetTypeId()));
    } else if (Protocol.compare("TcpIllinois") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpIllinois::GetTypeId()));
    } else if (Protocol.compare("TcpWestwood") == 0) { // the default protocol type in ns3::TcpWestwood is WESTWOOD
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpWestwood::GetTypeId()));
        Config::SetDefault("ns3::TcpWestwood::FilterType", EnumValue(TcpWestwood::TUSTIN));
    } else if (Protocol.compare("TcpWestwoodPlus") == 0) {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpWestwood::GetTypeId()));
        Config::SetDefault("ns3::TcpWestwood::ProtocolType", EnumValue(TcpWestwood::WESTWOODPLUS));
        Config::SetDefault("ns3::TcpWestwood::FilterType", EnumValue(TcpWestwood::TUSTIN));
    } else {
        NS_LOG_DEBUG("Invalid TCP version");
        exit(1);
    }

    // Create the network topology
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps")); // The data rate for the P2P star net devices should be set to 5Mbps
    pointToPoint.SetChannelAttribute("Delay", StringValue("10ms")); // The channel delay for the P2P star channels should be set to 10ms PointToPointStarHelper (uint32_t numSpokes,PointToPointHelper p2pHelper);

    // The topology will be made up of two PointToPointStar topologies
    PointToPointStarHelper sendStar(nSpokes, pointToPoint);
    PointToPointStarHelper receiveStar(nSpokes, pointToPoint);

    // Use a PointToPointHelper to create a link between the star’s hubs
    PointToPointHelper starhub;
    starhub.SetDeviceAttribute("DataRate", StringValue("1Mbps")); // The data rate should be set to 1Mbps
    starhub.SetChannelAttribute("Delay", StringValue("20ms")); // The channel delay should be set to 20ms
    NetDeviceContainer devices = starhub.Install(receiveStar.GetHub(), sendStar.GetHub()); // NetDeviceContainer = PointToPointHelper::Install(Ptr<Node> a, Ptr<Node> b)

    // Set up the ip address
    InternetStackHelper internet;
    sendStar.InstallStack(internet);    // Install internet for sendStar
    receiveStar.InstallStack(internet); // Install internet for receiveStar
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0"); // Set the base network number, network mask and base address.
    Ipv4InterfaceContainer interfaces = address.Assign(devices); // Assign Base Address to starhub
    receiveStar.AssignIpv4Addresses(Ipv4AddressHelper("10.2.1.0", "255.255.255.0")); // Assign Ipv4 Address to receiveStar
    sendStar.AssignIpv4Addresses(Ipv4AddressHelper("10.3.1.0", "255.255.255.0")); // Assign Ipv4 Address to receiveStar

    // Create packet sink on each spoke of receive star to receive packets.
    ApplicationContainer sinkApps;
    PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), 5000));
    for (uint32_t i = 0; i < nSpokes; ++i) {
        sinkApps.Add(sink.Install(receiveStar.GetSpokeNode(i))); // Use the PacketSinkHelper to install an instance of a packet sink on each of the spokes on the receiving star.
    }
    sinkApps.Start(Seconds(1.0)); // Have all of these applications start at time 1.0 second.

    // Install an instance of BulkSendApplication on each of the sending nodes.
    BulkSendHelper bulkSender("ns3::TcpSocketFactory", Address());
    bulkSender.SetAttribute("SendSize", UintegerValue(1024));
    ApplicationContainer sourceApps;

    for (uint32_t i = 0; i < nSpokes; ++i) {
        uint32_t remoteNode = (i + 1) % nSpokes;
        AddressValue remoteAddress(InetSocketAddress(receiveStar.GetSpokeIpv4Address(remoteNode), 5000));
        bulkSender.SetAttribute("Remote", remoteAddress);
        sourceApps.Add(bulkSender.Install(sendStar.GetSpokeNode(i)));
    }
    sourceApps.Start(Seconds(2.0)); // Set all of these applications to start at time 2.0 seconds

    // Turn on global static routing so we can actually be routed across the star.
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Have your simulation run for 60 seconds.
    Simulator::Stop(Seconds(60.0));
    Simulator::Run();

    uint32_t totalRx = 0;
    uint32_t endTime = 60.0;

    // Once the simulation has completed, prior to calling Simulator::Destroy(), you’ll need to collect data.
    for (uint32_t i = 0; i < nSpokes; ++i) {
        Ptr<PacketSink> sink = DynamicCast<PacketSink>(sinkApps.Get(i));
        uint32_t bytesReceived = sink->GetTotalRx();
        totalRx += bytesReceived;
        std::cout << "Sink " << i << "\tTotalRx: " << bytesReceived * 1e-6 * 8
                  << "Mb";
        std::cout << "\tGoodput: " << (bytesReceived * 1e-6 * 8) / endTime
                  << "Mbps" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Totals\tTotalRx: " << totalRx * 1e-6 * 8 << "Mb";
    std::cout << "\tGoodput: " << (totalRx * 1e-6 * 8) / endTime << "Mbps"
              << std::endl;

    Simulator::Destroy();

    return 0;
}
