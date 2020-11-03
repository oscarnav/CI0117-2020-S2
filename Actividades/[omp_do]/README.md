# Para que funciona

La directiva omp_do sirve para especificar las iteraciones de un dado ciclo para que sea ejecutado en paralelo por hilos en el mismo equipo. Es el equivalente a la directiva for para fortran.

Ejemplo:

real(8) :: A(1000), dummy(2:1000:2)
!Saves the even indices
!$OMP DO
do i = 2, 1000, 2
dummy(i) = A(i)
enddo
!$OMP END DO
