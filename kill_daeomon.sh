#!/bin/bash

# Замените "название_демона" на фактическое имя вашего демона
daemon_name=vlan_tagger

# Получить список всех PID, связанных с вашим демоном
pids=$(pgrep -f "$daemon_name")

# Проверить, есть ли какие-либо процессы для завершения
if [ -z "$pids" ]; then
    echo "Нет активных процессов для демона: $daemon_name"
    exit 1
fi

# Завершить все процессы
for pid in $pids; do
    echo "Остановка процесса с PID: $pid"
    kill $pid
done



echo "Все процессы демона $daemon_name были остановлены."
