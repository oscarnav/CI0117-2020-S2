# Integrantes:

Alejandro Ramirez B96367

Oscar Navarro B95549

## Explicacion de scheduling guiado
Este tipo de scheduling es similar al dinámico, pero con la diferencia de que el tamaño de los chunks cambia a medida que el programa corre. Empieza con chunks grandes y se va ajustando a tamaños más pequeños si la carga de trabajo no está balanceada.

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
| Cyclic (guided,1) | 7,166202689s |
| Cyclic (guided,cantidad de hilos) | 7,081352763s | 
| Cyclic (guided, mitad de hilos) | 7,087079142s | 
| Cyclic (guided, mitad  de iteraciones ) | 7,131716650s | 

## Conclusiones
Según nuestras pruebas realizadas el scheduling con menor tiempo de ejecución fue el scheduling dinámico con el valor N siendo igual a la mitad de hilos (5), nosotros consideramos que la razon por la cual obtuvo el mejor rendimiento fue por la cantidad de procesadores de la maquina dado que la mitad del total de hilos utilizados se aproxima más a la cantidad de procesadores el programa obtiene mejor rendimiento dado que los procesadores no requieren estar cambiando de hilos.



