#!/bin/bash


# Путь к вашему файлу s21_cat
S21_CAT="../s21_cat"


# Функция для тестирования
run_test() {
   local original_flag="$1"
   local input="$2"


   # Создание временного файла с входными данными
   local input_file=$(mktemp)
   echo -e "$input" > "$input_file"


   # Маппинг GNU-флагов на BSD-флаги
   local mapped_flag="$original_flag"
   case "$original_flag" in
       --number-nonblank)
           mapped_flag="-b" ;;
       --number)
           mapped_flag="-n" ;;
       --squeeze-blank)
           mapped_flag="-s" ;;
       -E)
           mapped_flag="-e -v" ;;
       -T)
           mapped_flag="-t -v" ;;
   esac


   # Запуск оригинального cat (GNU версия) и вашей программы
   local expected_output=$(env LC_ALL=C cat $mapped_flag "$input_file")
   local actual_output=$($S21_CAT $mapped_flag "$input_file")


   # Вывод результатов сравнения
   echo "cat output (flag $original_flag):"
   echo "$expected_output"
   echo "s21_cat output (flag $original_flag):"
   echo "$actual_output"


   # Сравнение результатов и возврат результата
   if [ "$expected_output" == "$actual_output" ]; then
       echo "PASS"
       echo "$original_flag: yes" >> results.tmp
   else
       echo "FAIL"
       echo "$original_flag: no" >> results.tmp
   fi


   # Удаление временного файла
   rm "$input_file"
}


# Тестовые сценарии
input_content="Line 1\n\nLine 3\n\tTabbed line\n"


# Удаление временного файла с результатами
> results.tmp


run_test "-b" "$input_content"
run_test "--number-nonblank" "$input_content"
run_test "-e" "$input_content"
run_test "-E" "$input_content"
run_test "-n" "$input_content"
run_test "--number" "$input_content"
run_test "-s" "Line 1\n\n\nLine 3\n"
run_test "--squeeze-blank" "Line 1\n\n\nLine 3\n"
run_test "-t" "$input_content"
run_test "-T" "$input_content"
run_test "-v" "[127] \n[128] �\n[129] �\n[130] �\n[131] �\n[132] �\n[133] �\n[134] �\n[135] �\n[136] �\n"


# Итоговый результат по всем флагам
echo "ВЫВОД:"
cat results.tmp


# Удаление временного файла с результатами
rm results.tmp
