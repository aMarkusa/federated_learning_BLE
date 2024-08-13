The central, or advertiser will:

1. Start scanning for peripherals with name FLP
2. Once it finds a device it will add it to a list
3. Once no more devices can be found within 5s -> stop scanning
4. Generate the training data and divide it
5. Connect to peripheral
6. Open L2CAP channel
7. Transfer the training data
8. Transfer the parameters
9. Start PAwR train
10. Disconnect from peripheral
11. Send out start command in PAwR
12. Wait for parameters from peripherals
13. Repeat