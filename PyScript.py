import dpkt
import matplotlib.pyplot as plt
from datetime import datetime

def analyze_packets(pcap_file):
    throughput_data = {}  # Dictionary to store throughput data for each TCP flow
    latency_data = {}  # Dictionary to store latency data for each TCP flow

    with open(pcap_file, 'rb') as file:
        pcap = dpkt.pcap.Reader(file)
        for timestamp, buf in pcap:
            # Process each packet and extract relevant information
            # Calculate throughput and latency for each TCP flow
            # Update throughput_data and latency_data dictionaries

    # Plotting
    for tcp_flow, throughput_values in throughput_data.items():
        plt.plot(throughput_values, label=f'TCP Flow {tcp_flow}')

    plt.xlabel('Experiment Number')
    plt.ylabel('Average Throughput (bits per second)')
    plt.legend()
    plt.show()

    for tcp_flow, latency_values in latency_data.items():
        plt.plot(latency_values, label=f'TCP Flow {tcp_flow}')

    plt.xlabel('Experiment Number')
    plt.ylabel('Average Latency (milliseconds)')
    plt.legend()
    plt.show()

if _name_ == "_main_":
    analyze_packets('captured_packets.pcap')