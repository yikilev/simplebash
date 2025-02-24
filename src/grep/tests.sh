COUNTER_SUCCESS=0
COUNTER_FAIL=0
TEST_FILE="test_files/test_*"
PATTERN_LIST="Chapter Intro Ver com cat for"
FLAGS_LIST="-v -i -n -l -c -h -o"

E_FLAG=(
    "-e Chapter -v -i $TEST_FILE"
    "-e Intro -i -n $TEST_FILE"
    "-e Ver -l -i $TEST_FILE"
    "-e cat -c -i $TEST_FILE"
    "-e com -v -n $TEST_FILE"
    "-e for -v -l $TEST_FILE"
    "-e Chapter -c -v $TEST_FILE"
    "-e Intro -n -l $TEST_FILE"
    "-e Ver -c -n $TEST_FILE"
    "-e com -c -l $TEST_FILE"
    "-f test_files/test_ptrn_grep.txt -c -l $TEST_FILE"
    "-f test_files/test_ptrn_grep.txt -v -i $TEST_FILE"
    "-f test_files/test_ptrn_grep.txt -i -n $TEST_FILE"
    "-f test_files/test_ptrn_grep.txt -l -i $TEST_FILE"
    "-f test_files/test_ptrn_grep.txt -c -i $TEST_FILE"
)
echo "=================================MANUAL TESTING========================================"
for PATTERN in "${E_FLAG[@]}"
do
    ./s21_grep $PATTERN > s21_grep.txt
    grep $PATTERN > grep.txt

    DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
    if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
    then
        (( COUNTER_SUCCESS++ ))
        echo "./s21_grep $PATTERN SUCCESS"
    else
        (( COUNTER_FAIL++ ))
        echo "./s21_grep $PATTERN FAIL"
    fi
done


echo "=================================START OF TESTING========================================"
for PATTERN in $PATTERN_LIST
do
    ./s21_grep $PATTERN $TEST_FILE > s21_grep.txt
    grep $PATTERN $TEST_FILE > grep.txt

    DIFF_RES="$(diff -s s21_grep.txt grep.txt)"

    if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
    then
        (( COUNTER_SUCCESS++ ))
        echo "./s21_grep $PATTERN $TEST_FILE   SUCCESS"
    else
        vimdiff grep.txt s21_grep.txt
        (( COUNTER_FAIL++ ))
        echo "./s21_grep $PATTERN $TEST_FILE   FAIL"
    fi
done

for PATTERN in $PATTERN_LIST
do
    for FLAG in $FLAGS_LIST
    do
        ./s21_grep $FLAG $PATTERN $TEST_FILE > s21_grep.txt
        grep $FLAG $PATTERN $TEST_FILE > grep.txt

        DIFF_RES="$(diff -s s21_grep.txt grep.txt)"

        if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
        then
            (( COUNTER_SUCCESS++ ))
            echo "./s21_grep $FLAG $PATTERN $TEST_FILE   SUCCESS"
        else
            (( COUNTER_FAIL++ ))
            echo "./s21_grep $FLAG $PATTERN $TEST_FILE   FAIL"
        fi
    done

    for FLAG1 in $FLAGS_LIST
    do
        for FLAG2 in $FLAGS_LIST
        do
            if [ $FLAG1 != $FLAG2 ] && { [ $FLAG1 != "-l" ] || [ $FLAG2 != "-c" ]; } && { [ $FLAG1 != "-c" ] || [ $FLAG2 != "-l" ]; }
            then 
                ./s21_grep $FLAG1 $FLAG2 $PATTERN $TEST_FILE > s21_grep.txt
                grep $FLAG1 $FLAG2 $PATTERN $TEST_FILE > grep.txt

                DIFF_RES="$(diff -s s21_grep.txt grep.txt)"

                if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
                then
                    (( COUNTER_SUCCESS++ ))
                    echo "./s21_grep $FLAG1 $FLAG2 $PATTERN $TEST_FILE   SUCCESS"
                else
                    (( COUNTER_FAIL++ ))
                    echo "./s21_grep $FLAG1 $FLAG2 $PATTERN $TEST_FILE   FAIL"
                fi
            fi
        done
    done
done

echo "=================================END OF TESTING=========================================="
echo "Total Success: $COUNTER_SUCCESS"
echo "Total Fail: $COUNTER_FAIL"
rm -r s21_grep.txt grep.txt