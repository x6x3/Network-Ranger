# 🌐 Network Ranger - WiFi Optimization Tool

A powerful C++ tool designed to optimize WiFi network performance, perform speed tests, and connect to nearby bridges for enhanced network connectivity.

##  Features

- **Network Discovery**: Scan and discover available WiFi networks
- **Speed Testing**: Perform comprehensive speed tests using Speedtest.net API
- **Network Optimization**: Optimize DNS, TCP, and WiFi settings for better performance
- **Bridge Connection**: Connect to nearby network bridges for extended coverage
- **Real-time Monitoring**: Monitor network performance in real-time
- **Network Analysis**: Analyze current network status and provide recommendations

## 📋 Requirements

- C++11 or higher
- libcurl (for HTTP requests)
- jsoncpp (for JSON parsing)
- POSIX-compliant system (Linux/macOS)

## 🔧 Installation

### Prerequisites

Install required dependencies:

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev libjsoncpp-dev build-essential

# macOS
brew install curl jsoncpp

# CentOS/RHEL
sudo yum install libcurl-devel jsoncpp-devel gcc-c++
```

### Compilation

```bash
# Compile the project
make

# Or compile manually
g++ -std=c++11 -o network_ranger ranger.c++ -lcurl -ljsoncpp -lpthread
```

## 🎯 Usage

Run the Network Ranger tool:

```bash
./network_ranger
```

### Menu Options

1. **Scan Available Networks** - Discover nearby WiFi networks
2. **Perform Speed Test** - Test download/upload speeds and ping
3. **Optimize Network** - Apply network optimizations
4. **Connect to Bridge** - Connect to nearby network bridges
5. **Analyze Network** - Analyze current network performance
6. **Start Monitoring** - Begin real-time network monitoring
7. **Show Network Info** - Display current network information
8. **Exit** - Close the application

## 🔧 Network Optimizations

The tool performs several optimizations:

- **DNS Optimization**: Uses Google DNS (8.8.8.8, 8.8.4.4) and Cloudflare DNS (1.1.1.1, 1.0.0.1)
- **TCP Optimization**: Increases TCP window size and enables TCP fast open
- **WiFi Optimization**: Selects optimal channels and adjusts transmit power
- **Bridge Connection**: Connects to nearby bridges for extended coverage

##  Speed Testing

The tool integrates with Speedtest.net API to provide accurate speed measurements:

- Download speed testing
- Upload speed testing
- Ping measurement
- Server selection
- ISP identification

## Bridge Connection

Network Ranger can detect and connect to nearby network bridges:

- Automatic bridge discovery
- Secure connection establishment
- Performance monitoring
- Automatic failover

##  Development

### Project Structure

```
Network:net/
├── ranger.c++          # Main application source
├── README.md           # Project documentation
├── Makefile           # Build configuration
└── .gitignore         # Git ignore rules
```

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/network-ranger.git
cd network-ranger

# Build the project
make

# Run the application
./network_ranger
```

##  Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

##  License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

##  Issues

If you encounter any issues or have suggestions, please open an issue on GitHub.

##  Support

For support and questions:
- Open an issue on GitHub
- Check the documentation
- Review the code comments
- contact me on @hexbypass


##  Disclaimer

This tool is for educational and personal use. Always ensure you have permission to access and modify network settings. The authors are not responsible for any network issues that may arise from using this tool.

---

**Made with ❤️ for better network performance** 