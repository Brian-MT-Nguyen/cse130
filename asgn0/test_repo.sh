

for test in `ls tests`; do
    echo "------------------------------------------------------------------------"
    printf "$test:\n"
    echo "------------------------------------------------------------------------"

    output=$(./tests/$test)
    return_code=$?
    if [ $return_code -eq 0 ]; then
	printf "SUCESS\n\n"
    else
	printf "FAILED\n\n"
	printf "Return code: $return_code\n"
	printf "Output:\n$output\n\n"
    fi

done
