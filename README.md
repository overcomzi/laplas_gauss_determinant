

# Консольное приложение по вычислению определителя матрицы
## Краткая информация

Приложение вычисляет определитель 2-мя способами: с помощью формулы Лапласа (рекурсивно) и с помощью метода Гаусса. Все промежуточные результаты тоже отображаются.  

Пример работы:  
![Скриншот результатов приложения](../assets/assets/demo1.png?raw=tru)  

## Запуск приложения
*main64.exe* - для 64 бит Windows  
*main32.exe* - для 32 бит Windows  
*myapp* - для Debian  

В коммандой строке выполните команду:  
`main64.exe input-matrix`  

где *input-matrix* - название файла с матрицей  
К примеру: main64.exe matrix.txt  
matrix.txt:
```
4
1 0 2 1
2 1 3 1
3 0 0 1
1 1 1 1
```  
1-ая строка - размер матрицы (4 x 4).  
Остальные 4 строчки представляют собой строки матрицы  

#### Определитель с помощью рекурсивного разложения
Почитать про формулу Лапласа можно здесь:  
http://elisey-ka.ru/algem/63.htm  

В приложении перед матрицей в выражении в скобках пишется номер строки.  
Выражение - операция над матрицей, к примеру:  
`(4) - 1 * (2)`  
Означет "вычесть из 4-ой строки 2-ую строку, домноженную на (-1)"  
![Скриншот результатов приложения выражения](../assets/assets/example-explanation-expression.png?raw=tru)  

### Определитель с помощью метода Гаусса
Почитать про метод Гасса для нахождения определителя можно здесь:  
https://pro-prof.com/forums/topic/gauss-determinant
  
В приложении сначала пишется коэффициент a(i,j), умноженный на (-1)^(i + j), затем соответственно "подматрица", определитель которой и будет миноном исходной матрицы.   

После равно идёт вычисление произведение этого коэффициента на минор.  
![Скриншот результатов приложения выражения](../assets/assets/example-explanation-laplas.png?raw=tru)

### Опционально можно заново с компилировать приложение
Dockerfile - уже готовый, необходимо лишь собрать образ c помощью такой команды:  
`docker image build -t gcc-compiler-image .`  
Дальше достаточно запустить скрипт *compile.bat*. Он создаст исполняемые файлы в директории *out*.