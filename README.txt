LOUKAS PETEINARIS sdi1600134

Important Disclaimers:
0. The app is not functional. I have implemented the whole Sockets part, and the
    Threads part but they are not working properly. As the code complexity escalated
    some Socket writing-reading failed (with errno 104) and the whole program crashes.
    I never got to debug this, and my whole schedule went down.
    I did not spend as much time as it required due to employment, others lessons
    and because I got infected to Covid19.
    I truly have learned a lot about Pipes, Sockets, Threads etc in this course
    and I truly believe my effort in all of these Projects is worth the minimum for
    succeeding in this class.
    Please be generous at my grading.

1. The code consists of various classes and the code is (at my best effort) self-
    explanatory. Due to lack of time, this was not made everywhere, and in these
    cases I have added according comments to help you understand.

2. All classes except MonitorServer, TravelMonitorClient, Socket and BloomFilterLinkedList
     were copied from Project1 and Project2, and only slightly changed for
    easier handling in this assignment.

3. Socket handling was done in a class with functions supporting both server and
    client. So both TravelMonitor and MonitorServer have a Socket object and call
    the appropriate functions. The functions also support moving data in chunks for
     a small buffer size by reading/writing a chunk of bufferSize eachtime and them
     copy it with memcpy(). In Socket class we have read/write for:
        - int
        - string: in order to send a string, firstly we send it's size (+1 for '\0')
            and then the string with
        - arrayOfInts
        - bitArray: in order to send the bitArray we first have to send it's field 'bitArray'
            which is an arrayOfInts, because it's given as a pointer and passing the pointer
            is wrong in sockets
        - BloomFilter: in order to send the BloomFilter, we first send the countryName
            and virusName and then it's BitArray because these are pointers

    In all these functions there is an argument 'isFromServer'. This is to support
    the class from both server and client and it is used to separate if the fd returned
    from acceptSocket() or listenSocket() will be used.

4. I assume for reasons of simplicity (and according to https://piazza.com/class/kkx2o2gvper2nj?cid=172)
    that socketBufferSize is bigger or equal than the size of an int. Sockets
    are initialized with socketBufferSize

5. If the socketBufferSize of the Sockets is smaller than the size of the message, then we transfer
    it in chunks. We read a small chunk of the total bytes until we reach total size
    and then add these bytes to the variable to be returned. For example check
    Socket::readStringInChunks();

7. For quickly combining the Project1's and Project2's design with this one I add to BF the Virus
    and Country that concern it. So that the TravelMonitorClient has a LinkedList of BFs
    and finds the appropriate one by the Virus and Country. This was the fastest way
    as I did not want to lose much time adjusting the Project1 code and I wanted to
    concentrate to Sockets and Threads.

8. For passing the BloomFilters from Monitors to TravelMonitorClient the protocol is:
    a. Pass the number of BloomFilters per MonitorServer, which is extracted from
        the combination of Virus-Country found in the files
    b. Until all expected BloomFilters arrive, use poll() to read from multiple
        Monitors, whatever comes first.

9. I have created the skeleton for parsing the User's requests and returning mock answers.