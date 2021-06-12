mkdir -p tests/

for (( i=1; i <= 40 ; i++  ))
do 
	./randomized-gen.out > tests/${i}.in
done


mkdir -p sols/

for (( i=1; i <= 40; i++ ))
do
	./four-russian-optimization.out < tests/${i}.in > sols/${i}.sol 
	if [ 0 -ne $? ]
	then
		echo Error at running ${i}
	fi
done 

for (( i=1; i<= 40; i++ ))
do
	./lcs_checker.out tests/${i}.in sols/${i}.sol
	if [ 0 -ne $? ]
	then
		echo Failed ${i}
	fi
done 


