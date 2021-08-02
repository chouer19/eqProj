sleep 30
./home/lbs/xuechong/rtkShare/build /dev/ttyS0 115200
result=$?
echo ${result}
#while  [ result -eq 0 ] 
while  [ $result == 0 ] 
do
    sleep 10
    ./home/lbs/xuechong/rtkShare/build /dev/ttyS0 115200
    result=$?
    echo ${result}
done


