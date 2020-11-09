# Integrantes:

Alejandro Ramirez B96367

Oscar Navarro B95549

## Explicacion de scheduling guiado
Este tipo de scheduling es similar al dinamico pero con la diferencia de que el tamaño de los chunks cambia a medida que el programa corre. Empieza con chunks grandes y se va ajustando a tamaños más pequeños si la carga de trabajo no esta balanceada.

## Cuadro comparativo

| Prueba | Tiempo de ejecucion |            
| --- | --- |
| Serial | 8,506385308s |
| Parallel | 8,389533284s |
| Cyclic (static,1) | 8,214059574s |
| Cyclic (static,cantidad de hilos) | 8,155760084s | 
| Cyclic (static, mitad de hilos) | 7,059650245s| 
| Cyclic (static, mitad  de iteraciones ) | 7,061183935s | 
| Cyclic (dynamic,1) | 8,020856127 |
| Cyclic (dynamic,cantidad de hilos) | 7,075531187s | 
| Cyclic (dynamic, mitad de hilos) | 7,0469472159s | 
| Cyclic (dynamic, mitad  de iteraciones ) | 7,102752436s | 
| Cyclic (guided,1) | 8,020856127s |
| Cyclic (guided,cantidad de hilos) | 7,075531187s | 
| Cyclic (guided, mitad de hilos) | 7,0469472159s | 
| Cyclic (guided, mitad  de iteraciones ) | 7,102752436s | 

## Conclusiones



