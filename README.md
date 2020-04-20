# Dynamic CVRPTW
Dynamic Capacitated Vehicle Routing Problem with Time Windows

Научно-исследовательская работа

Позволяет генерировать входные данные, запускать построение маршрутов и симулировать процесс доставки товаров с помощью оконного приложения. 

Сценарий использования:

1) Выберите файл с входными данными. Формат файла описан далее
2) Нажмите кнопку "Загрузить".
3) Выберите загруженный файл и нажмите кнопку "Запустить построение" для первого построения маршрутов.
4) Нажмите кнопку "Запустить доставку для последнего построенного маршрута"
5) Наблюдайте за доставкой и создавайте события при помощи формы справа.

По умолчанию виртуальное время идёт с той же скоростью, что и реальное. Виртуальное время можно ускорять и ставить на паузу. Цели, которые еще не удалены и не посещены можно удалять, менять временные окна и передвигать. Для обновления значений матрицы расстояний загрузите файл, формат которого описан ниже.

## Формат файла с входными данными

*количество_целей*

*количество_транспортных средств*

*ограничение_на_дальность_полёта*

*координата_цели_1_x координата_цели_1_y*

...

*координата_цели_N_x координата_цели_N_y*

*матрица (N+1)x(N+1) с расстояниями между депо и целями. Депо имеет номер 0*

*начало_временного_окна_1 конец_временного_окна_1*

...

*начало_временного_окна_N конец_временного_окна_N*

## Формат файла для обновления матрицы расстояний

*количество изменений*

*v_1 u_1 новое_расстояние_1*

...

*v_n u_n новое_расстояние_n*

Примеры можно найти по этому пути: https://github.com/Alexandr-TS/DynamicCVRPTW/tree/master/InputGenerator/new_tests