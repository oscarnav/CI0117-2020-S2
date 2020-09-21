-Descripcion del problema:
Santa Claus duerme en su taller en el Polo Norte y y solo puede ser despertado 
si sus nueve renos han vuelto de vacaciones o por los elfos que tengan problemas 
en la elaboracion de juguetes. Para dejar que Santa descanse los elfos solo 
lo pueden despertar cuando hayan 3 de ellos con problemas. Mientra esos 3 elfos se 
encuentran siendo atendidos, los demás elfos deberan esperar a que estos terminen para 
poder pedir ayuda a Santa. Si Santa despierta y encuentra a 3 de sus elfos pidiendo 
ayuda pero tambien  9 de sus renos ya volvieron de vacaciones, Santa dejara esperando 
a los elfos para atender primero a los renos porque es más importante alistar el 
trineo. Como los renos quieren aprovechar sus vaciones al máximo, su tiempo minimo
en regresar a trabajar es mayor al de los elfos, pero como los elfos trabajan y 
pueden necesitar ayuda espontanea, entonces su rango de tiempo es mayor al de los renos.


-¿Se puedo presentar alguna situacion de deadlock o starvation?
En este caso pensamos que se podria presentar starvation dado que mientras alguno de los elfos esperan otros podrian ganarle en recibir ayuda entonces este se quedaria esperando ser ayudado.
