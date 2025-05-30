#!/bin/bash




# Путь к файлу s21_grep
S21_GREP="../s21_grep"




# Функция для тестирования
run_test() {
   local test_number="$1"
   local flag="$2"
   local pattern="$3"
   local input="$4"
   local input2="$5"




   # Создание временного файла с входными данными
   local input_file=$(mktemp)
   echo -e "$input" > "$input_file"
   local input_file2=$(mktemp)
   echo -e "$input2" > "$input_file2"




   # Запуск оригинального grep и программы
   local expected_output=$(env LC_ALL=C grep $flag "$pattern" "$input_file" "$input_file2" 2>/dev/null)
   local actual_output=$($S21_GREP $flag "$pattern" "$input_file" "$input_file2" 2>/dev/null)




   # Вывод результатов сравнения
   echo -e "\033[90m####################################################################################\033[0m"
   echo -e "\033[95mTest #$test_number\033[0m"
   echo -e "\033[93mgrep output (flag $flag):\033[0m"
   echo "$expected_output"
   echo -e "\033[90m###########################\033[0m"
   echo -e "\033[93ms21_grep output (flag $flag):\033[0m"
   echo "$actual_output"




   # Сравнение результатов и возврат результата
   if [ "$expected_output" == "$actual_output" ]; then
       echo -e "\033[92mPASS\033[0m"
       printf "%2d: %4s: \033[92mYes\033[0m\n" "$test_number" "$flag" >> results.tmp
   else
       echo -e "\033[91mFAIL\033[0m"
       printf "%2d: %4s: \033[91mNo\033[0m\n" "$test_number" "$flag" >> results.tmp
   fi




   # Удаление временного файла
   rm "$input_file"
   rm "$input_file2"
}




# Удаление временного файла с результатами
> results.tmp




# Тестовые сценарии
input_content="Hello world\nhello World\nHELLO WORLD\nThis is a test\nTest case\n"
input_content2="hello world\bye World\nBYE WORLD\nThis is a test\nTest case\n"






# Тесты для различных флагов
test_number=1
run_test "$test_number" "-e" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-i" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-v" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-c" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-l" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-n" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-h" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-s" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-in" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-ic" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-vn" "hello" "$input_content" "$input_content2"; ((test_number++))
run_test "$test_number" "-ie" "hello" "$input_content" "$input_content2"; ((test_number++))




# Подготовка файла с шаблонами для флага -f
echo -e "hello\nTest" > patterns.tmp
run_test "$test_number" "-f" "patterns.tmp" "$input_content" "$input_content2"; ((test_number++))
rm patterns.tmp




run_test "$test_number" "-o" "hello" "$input_content" "$input_content2"; ((test_number++))




# Итоговый результат по всем флагам
echo -e "\033[90m####################################################################################\033[0m"
echo -e "\033[93mВЫВОД:\033[0m"
cat results.tmp




# Удаление временного файла с результатами
rm results.tmp
