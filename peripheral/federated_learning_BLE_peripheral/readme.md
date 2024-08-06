# Wireless Federated learning peripheral

## Introduction

This project serves as the peripheral, or trainer, in the wireless fl PoC. This role has the following requirements:

1. Receive training data and starting parameters from the central device via BLE
2. Train a predetermined model on this training data
3. Send the optimized parameters back to the central via BLE
4. Wait for new global parameters

## BLE functional details

### Receiving training data and starting parameters

The training data can be quite large (several kB), so connection-based communication is required. Using OTS would be beneficial, but to get going the dataset will be split according to the MTU and written in chunks from the central to peripheral. During this process the starting parameters will also be written to the peripheral.

### Sending optimized parameters and receiving global parameters

As the parameters are relatively small in size, PAwR will be used for sending and receiving parameters. This enables the central to speak with multiple peripheral effectively.

## Custom GATT Services

The peripheral will include the following custom services and characteristics in the GATT database

1. **Training data service** (6e4fd68b-8cc4-4006-ab83-555fbcd6e10c)

    1. **Feature vector** (d98d76d4-58fb-4946-b11a-55c6956bcf71)

        * 2-dimensional uint16_t array

    2. **Label vector** (0b17a09a-a33e-415f-977f-f401b55bc21f)

        * 2-dimensional uint16_t array

2. **Training data metadata service** (7cede457-58b3-49ed-8018-da3321c2fb90)

    1. **Feature vector max length** (b01191e2-1c71-4fa2-a7df-fcf32aff0e99)

        * uint16_t

    2. **Label vector max length** (7003a8c6-a669-4668-9a46-9b4bfcc9df7a)

        * uint16_t

    3. **Feature vector length** (78ac1d5d-2bcf-41fc-8a5e-64bc2016e74c)

        * Describes how long the actual vector is
        * uint16_t

    4. **Label vector length** (88e199d9-45a6-43d6-bf42-7df79a3a56b8)

        * Describes how long the actual vector is
        * uint16_t

3. **Machine learning model service** (c1c8580d-0c54-4f49-bf31-58404adde68a)

    1. **Model parameters** (dc7fd511-6425-4cbd-a21f-837f4c344bf7)

        * uint16_t array

    2. **Model** (fedae635-703a-4c9a-8720-d35241048fed)

        * Enum describing the model

    3. **Hyperparameters** (7e76dec4-7655-44ea-8dee-84db34d963db)

        * uint16_t array