Для использования утилиты генерации тестов необходимо скомпилировать файл InputGenerator.cpp.
Использование утилиты поддерживает 2 опции:
	1) rand dronsCnt targetsCnt [fileName]
		Генерирует случайные точки. Если файл не указан, выводит результат в консоль. Иначе результат выводится в fileName.txt
	2) clusters dronsCnt targetsCnt clustersCnt [fileName]
		Генерирует случайные точки, разбитые на классы (компактные группы на плоскости)
Пример запуска: a.exe clusters 4 20 2 test


Результат имеет следующий формат:
	Количество_дронов
	Количество_целей
	Максимальная_дальность_полёта
	x_1 y_1
	...
	x_n y_n

	timeWindowStart_1 timeWindowEnd_1
	...
	timeWindowStart_n timeWindowEnd_n
