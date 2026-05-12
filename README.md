# Proga

КИ23-02/2Б
Жуков Матвей



Инструкция по сборке:

&#x09;Предварительные требования

* Компилятор – Visual Studio (например, 2019/2022) с инструментами C++.
* CMake (версия 3.16 или выше).
* Qt6 – установлен, известен путь к нему, например D:\\Qt\\6.5.3\\msvc2022\_64.



&#x09;Команды CMake

cd D:\\<ваш путь>\\lab2

mkdir build

cd build

cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE\_PREFIX\_PATH="D:\\Qt\\6.5.3\\msvc2022\_64"

cmake --build . --config Release

Release\\lab2.exe

