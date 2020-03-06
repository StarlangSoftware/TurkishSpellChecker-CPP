# SpellChecker

Yazım denetleyici, verilen metindeki yazım hatalarını bulup düzelten Nlptoolkit bilşenidir. Her kelime için hatayı tespit edip olası doğru adaylar arasından seçim yapar. Bu bileşen iki farklı yazım denetleyici içermektedir. Bunlar basit yazım denetleyici (simple spell checker) ve n-karakter yazım denetleyicidir (ngram spell checker).

Basit yazım denetleyici, basit geri dönüştürücü ile benzer bir yöntem kullanır. Girdideki her kelime için her karakter gezilip bu karakter olası bütün karakterlerle değiştirilerek mümkün olabilecek bütün kelimeler oluşturulur ve bunlardan biçimbilimsel olarak çözümlemenebilenlerden bir tanesi rassal olarak seçilir.

N-karakter yazım denetleyici, benzer şekilde n-karakter geri dönüştürücü ile aynı mantığı kullanmaktadır. Önce, basit yazım denetleyicide olduğu gibi kelimeler için aday listeleri hazırlanır. Daha sonra ise n-karakter modelinden bu adaylar için olasılıklar hesaplanarak, her kelime için olasılığı en yüksek olan aday çıktı olarak verilir.

For Developers
============
You can also see either [Python](https://github.com/olcaytaner/TurkishSpellChecker-Py) 
or [Java](https://github.com/olcaytaner/TurkishSpellChecker) repository.
## Requirements

* [C++ Compiler](#cpp)
* [Git](#git)


### CPP
To check if you have compatible C++ Compiler installed,
* Open CLion IDE 
* Preferences >Build,Execution,Deployment > Toolchain  

### Git

Install the [latest version of Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git).

## Download Code

In order to work on code, create a fork from GitHub page. 
Use Git for cloning the code to your local or below line for Ubuntu:

	git clone <your-fork-git-link>

A directory called TurkishSpellChecker-CPP will be created. Or you can use below link for exploring the code:

	git clone https://github.com/olcaytaner/TurkishSpellChecker-CPP.git

## Open project with CLion IDE

To import projects from Git with version control:

* Open CLion IDE , select Get From Version Control.

* In the Import window, click URL tab and paste github URL.

* Click open as Project.

Result: The imported project is listed in the Project Explorer view and files are loaded.


## Compile

**From IDE**

After being done with the downloading and opening project, select **Build Project** option from **Build** menu. After compilation process, user can run TestSpellChecker.cpp .
