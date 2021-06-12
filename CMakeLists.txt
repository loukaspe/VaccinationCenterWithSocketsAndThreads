cmake_minimum_required(VERSION 3.19)
project(VaccinationCenterUsingSocketsAndThreads)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(
        monitorServer
        monitorServer.cpp
        BloomFilter.cpp BloomFilter.h
        BitArray.cpp BitArray.h
        Helper.cpp Helper.h
        NotVaccinatedPersonSkipListNode.cpp NotVaccinatedPersonSkipListNode.h
        NotVaccinatedSkipList.cpp NotVaccinatedSkipList.h
        VaccinatedPersonSkipListNode.cpp VaccinatedPersonSkipListNode.h
        VaccinatedSkipList.cpp VaccinatedSkipList.h
        Person.cpp Person.h
        Vaccination.cpp Vaccination.h
        PersonLinkedList.cpp PersonLinkedList.h
        PersonLinkedListNode.cpp PersonLinkedListNode.h
        VirusLinkedList.cpp VirusLinkedList.h
        VirusLinkedListNode.cpp VirusLinkedListNode.h
        MenuPromptCreator.cpp MenuPromptCreator.h
        Virus.cpp Virus.h
        CitizenRecordsFileReader.cpp CitizenRecordsFileReader.h
        Date.cpp Date.h
        VaccinationCenter.cpp VaccinationCenter.h
        Country.cpp Country.h CountryLinkedList.cpp CountryLinkedList.h CountryLinkedListNode.cpp CountryLinkedListNode.h
        MonitorServer.cpp MonitorServer.h
        BloomFilterLinkedListNode.cpp BloomFilterLinkedListNode.h BloomFilterLinkedList.cpp BloomFilterLinkedList.h
        Socket.cpp Socket.h)

add_executable(
        travelMonitorClient
        main.cpp
        BitArray.cpp BitArray.h
        BloomFilter.cpp BloomFilter.h
        Helper.cpp Helper.h
        NotVaccinatedPersonSkipListNode.cpp NotVaccinatedPersonSkipListNode.h
        NotVaccinatedSkipList.cpp NotVaccinatedSkipList.h
        VaccinatedPersonSkipListNode.cpp VaccinatedPersonSkipListNode.h
        VaccinatedSkipList.cpp VaccinatedSkipList.h
        Person.cpp Person.h
        Vaccination.cpp Vaccination.h
        PersonLinkedList.cpp PersonLinkedList.h
        PersonLinkedListNode.cpp PersonLinkedListNode.h
        VirusLinkedList.cpp VirusLinkedList.h
        VirusLinkedListNode.cpp VirusLinkedListNode.h
        MenuPromptCreator.cpp MenuPromptCreator.h
        Virus.cpp Virus.h
        CitizenRecordsFileReader.cpp CitizenRecordsFileReader.h
        Date.cpp Date.h
        VaccinationCenter.cpp VaccinationCenter.h
        Country.cpp Country.h CountryLinkedList.cpp CountryLinkedList.h CountryLinkedListNode.cpp CountryLinkedListNode.h
        TravelMonitorClient.cpp TravelMonitorClient.h
        BloomFilterLinkedListNode.cpp BloomFilterLinkedListNode.h BloomFilterLinkedList.cpp BloomFilterLinkedList.h
        Socket.cpp Socket.h
)
