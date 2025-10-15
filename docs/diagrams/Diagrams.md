# Содержание
1 [Диаграмма классов](#classes)  
2 [Диаграммы активностей](#activities)  
2.1 [Проверка файлов/папки на вирусы](#scan_activity)  
2.2 [Просмотр результатов сканирования](#report_activity)  
2.3 [Настройка фильтров](#filter_activity)  
3 [Диаграммы последовательностей](#sequences)  
3.1 [Проверка файлов/папки на вирусы](#acan_sequence)  
3.2 [Просмотр результатов сканирования](#report_sequence)  
3.3 [Настройка фильтров](#filter_sequence)  
4 [Диаграммы состояний](#states)  
4.1 [Проверка файлов/папки на вирусы](#scan_state)  
4.2 [Просмотр результатов сканирования](#report_state)  
4.3 [Настройка фильтров](#filter_state)  
5 [Диаграмма компонентов](#components)  
6 [Диаграмма использований](#use_case)  
7 [Глоссарий](#glossary)  
8 [Поток событий](#flow_of_events)   

<a name="classes"/>

# 1 Диаграмма классов

![Диаграмма классов](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/ClassDiagram.png)

<a name="activities"/>

# 2 Диаграммы активностей

<a name="scan_activity"/>

## 2.1 Проверка файлов/папки на вирусы

![Диаграмма активности сканирования файлов](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/ScanActivityDiagram.png)

<a name="report_activity"/>

## 2.2 Просмотр результатов сканирования

![Диаграмма активности просмотра результатов](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/ReportActivityDiagram.png)

<a name="filter_activity"/>

## 2.3 Настройка фильтров

![Диаграмма активности настройки фильтров](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/FilterActivityDiagram.png)

<a name="sequences"/>

# 3 Диаграммы последовательностей

<a name="scan_sequence"/>

## 3.1 Проверка файлов/папки на вирусы

![Диаграмма последовательности сканирования файлов](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/ScanSequenceDiagram.png)

<a name="report_sequence"/>

## 3.2 Просмотр результатов сканирования

![Диаграмма последовательности просмотра результатов](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/ReportSequenceDiagram.png)

<a name="add_track_sequence"/>

## 3.3 Настройка фильтров

![Диаграмма последовательности настройки фильтров](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/FilterSequenceDiagram.png)

<a name="states"/>

# 4 Диаграммы состояний

<a name="scan_state"/>

## 4.1 Проверка файлов/папки на вирусы

![Диаграмма состояний сканирования файлов](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/ScanStateDiagram.png)

<a name="report_state"/>

## 4.2 Просмотр результатов сканирования

![Диаграмма состояний просмотра результатов](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/ReportStateDiagram.png)

<a name="filter_state"/>

## 4.3 Настройка фильтров

![Диаграмма состояний настройки фильтров](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/FilterStateDiagram.png)

<a name="components"/>

# 5 Диаграмма компонентов

![Диаграмма компонентов](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/ComponentDiagram.png)

<a name="use_case"/>

# 6 Диаграмма использований

![Диаграмма использований](https://github.com/skritigus/LutiVirusScanner/blob/main/docs/diagrams/images/UseCaseDiagram.png)

<a name="glossary"/>

# 7 Глоссарий


| Термин                        | Определение                                                                                                                 |
|:---|:---|
| API-ключ                  | Уникальный токен пользователя, необходимый для доступа к VirusTotal API												                                                                               |
| Отчёт | Информация о сканировании, включающая результаты всех антивирусных движков   									                                                                |
| Фильтр                    | Правило, ограничивающее список файлов для сканирования 									                                                             |  
| VirusTotal API           | Внешний сервис для анализа файлов |
| Контроллер                         | Слой, управляющий действиями и логикой взаимодействия между UI и сервисами 													                                                                                     |

<a name="flow_of_events"/>

# 8. Поток событий

<a name="log_in_event"/>

