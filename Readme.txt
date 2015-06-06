This code written by Lin Bo performs distributed calculator in C++ .

To compile simply run ¡°make¡±. 
Two executable files, ¡°server¡± and ¡°client¡± will be generated.

To make a connection:
1)run ¡°./server¡±+ port number
2)run ¡±./client¡± + hostname + port number

To perform calculation:
The client will prompt you to enter an operator followed by two operands (single space separated):
[PLUS|MINUS|TIMES|DIVIDE] operand1 operand2
(e.g. ¡°PLUS 1 3¡±)
Each operand should be a number in the range [0,65535].
After calculation made by server, the client will print the answer prefixed by ¡°RESULT¡±
 (e.g. ¡°RESULT 4¡±)
Enter 'OK CLOSE' to exit.

To test:
Several test cases have been provided in ¡°test_in.txt¡±.
Simply uncomment the test section (marked by /*--for test--*/ and /*--for test end¡ª*/) in client.cpp to test.
The output file is ¡°test_out.txt¡±.
