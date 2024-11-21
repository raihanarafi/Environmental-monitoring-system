# Environmental-monitoring-system
1. Description - This Environmental Monitoring System collects and processes temperature, humidity, and dew point data from an external API. The system logs the data, analyzes it, and sends email alerts when the temperature or dew point exceeds certain thresholds.
2. Installation Instructions -
   1. Clone the repository: git clone https://github.com/yourusername/Environmental-monitoring-system.git
   2. Navigate to the project directory: cd Environmental-monitoring-system
   3. Install required libraries: You'll need the libcurl and libjson-c libraries for making HTTP requests and parsing JSON data. On Ubuntu or Debian-based systems, you can install the required libraries with the following command: sudo apt-get install libcurl4-openssl-dev libjson-c-dev
   5. Compile the project: Use the make command to compile the project: make
   6. Run the program: Once the project is compiled, you can run the program to start monitoring the temperature and humidity: ./bin/weather_monitor .The system will start fetching real-time data from the weather API, logging the data, and sending email alerts based on the temperature and dew point thresholds.
