# Integrantes:

Alejandro Ramirez B96367

Oscar Navarro B95549

## Nota
Las pruebas fueron realizadas con un numero como máximo de 10 000 000 esto debido a que el hardware hacía que el tiempo de ejecución fuese excesivamente grande para numeros mayores a este.


## Cuadro comparativo

| Prueba | 1 | 16 | 32 | 64       
| --- | --- | --- | --- |--- |
| count_primes_process | 15.027637s | 7.989519s | 7.901880s | 9.954613s|
| count_primes_reduction | 14.940892s | 10.558342s | 10.258249s | 11.117398s|
| count_primes_all_reduce | 7.030014s | 2.728543s | 1.856697s | 1.527496s|


