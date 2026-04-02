# 🐚 Tinsh Shell: Mimari ve Kod Analizi

Bu döküman, sıfırdan geliştirdiğimiz Unix/Linux tabanlı `myshell` projesinin arka planındaki mekanizmayı, her bir dosyanın ne işe yaradığını ve C dilindeki temel bilgisayar bilimleri konseptlerini açıklamaktadır.

> [!NOTE]
> Yazdığımız shell, standart bir Linux terminalinin (`bash` veya `zsh`) temel fonksiyonlarının minimal ama tam çalışır bir simülasyonudur. Sistemdeki her şey **"Oku -> Ayrıştır -> Komutu Değerlendir"** döngüsüyle çalışır.

***

## 📂 1. `main.c` (Ana Motor ve Karar Merkezi)

Bu dosya sistemimizin kalbidir. Program çalıştığı sürece durmadan tekrar eden bir sonsuz döngü (`while(1)`) barındırır. Linux komut satırının açık kalmasını sağlayan işlem budur.

**Temel Görevleri:**
- **Prompt (Girdi Satırı):** En son eklediğimiz şık görünümü burada çizdiririz. `getenv("USER")` ile giriş yapmış olan kullanıcının adını bulur, `getcwd` ile bulunduğumuz klasörü alır ve renkli ANSI kodlarıyla `kullanıcı@myshell:/klasör$` basar.
- **`fgets` ile Okuma:** Klavyeden yazılan tüm harfleri enter'a basılana kadar (veya `Ctrl+D` ile `EOF` sinyali gelene kadar) bir karaktere dizisine (`input`) doldurur.
- **Sinyal Yönetimi (Signal Handling):** Kullanıcı `Ctrl+C`'ye (`SIGINT`) bastığında programın kendi kendini öldürmemesini (`perror`), sadece o an yazılan satırı söküp atarak yeni satıra geçmesini sağlar.
- **Yönlendirme:** Gelen komutları okuduktan sonra `parse_input`'a yollar, sonra `is_builtin` yardımıyla bu komutun *(kendi komutumuz mu yoksa dış bir uygulama mı)* olduğunu tespit eder.

***

## ✂️ 2. `parser.c` ve `parser.h` (Ayrıştırıcı Kelime Analizcisi)

Kullanıcı terminale `echo "merhaba dunya" test` gibi bir şey yazdığında, bilgisayar bunun neresinin komut, neresinin argüman olduğunu bilemez. Bu dosya tam olarak bunu çözer.

**Temel Görevleri:**
- **`trim_whitespace`:** Başlangıçtaki veya sondaki tüm gereksiz boşluk tuşu girişlerini (`space`) temizler.
- **Tırnak Hassasiyeti (`parse_input`):** `echo` komutunun içinde örneğin çift tırnak (`" "`) barındıran bir metin algılarsa, o metnin içindeki kelimeleri boşluklardan bölmez. Onları tek bir kelime/argüman (`token`) olarak kabul eder ve alıntı işaretlerini temizler.
- Komutları parçalara ayırarak bir **dizi (Array)** formuna sokar. (Örn: `tokens[0] = "echo"`, `tokens[1] = "merhaba dunya"`)

***

## ⚙️ 3. `executor.c` ve `executor.h` (Dış İşlem Çalıştırıcı & İşletim Sistemi Köprüsü)

Eğer yazdığımız komut bizim kendi küçük evrenimizde (`builtins.c`) olmayan bir dış komutsa (örneğin işletim sisteminin `/bin/ls` komutuysa), bu dosya devreye girer. Unix dünyasının efsanevi **Process Yönetimi** burada döner.

> [!IMPORTANT]
> Bu modüldeki konsept doğrudan İşletim Sistemleri (Operating Systems) teorisine aittir.

**Temel Görevleri:**
- **`fork()`:** İşletim sistemine şu anda çalışan kabuğumuzun anında bir "Birebir Klon" (`Child Process`) kopyasını oluşturmasını emreder.
- **`execvp()`:** Klon üretilir üretilmez, asıl yazdığımız Unix komutunu (Örn: `python3`) bulmak için `PATH` ortam değişkenlerini arar ve bulduğu uygulamanın ruhunu o klonun içine kopyalar.
- **`waitpid()`:** Asıl programımız (bizim yazdığımız `myshell`), yavru-klon tamamen işlemlerini bitirip kapanana (`exit()`) kadar donmuş şekilde bekler ve sonra kontrolü eline tekrar geri alır.

***

## 🧠 4. `builtins.c` ve `builtins.h` (Yerleşik/Dahili Shell Komutları)

Dışarıdaki `fork` işlemleri harika olsa da, bazı komutların dışarıdaki klonda işlem görmesi **imkansızdır**. Örneğin `cd` (klasör değiştirme) ile klasör değiştirirseniz, sadece klonun klasörü değişmiş olur ve klon ölünce siz aslında terminalde hâlâ eski klasörde kalırsınız! Bu yüzden `cd` gibi komutlar asıl shell process'in içine yazılmak ZORUNDADIR.

**Temel Görevlerimiz:**
- **`builtin_cd`:** `chdir()` fonksiyonunu çağırarak ana uygulamanın dosya gezgini dizinini günceller.
- **`builtin_pwd`:** Nerede olduğumuzu basmak için `getcwd()` adresini okuyup print eder.
- **`builtin_echo`:** Parser'dan aldığı kelimeleri, aralarında bir boşluk bırakarak saniyeler içinde ekrana yansıtır.
- **`builtin_whoami`:** Son eklediğimiz yapı. `getenv("USER")` foksiyonu ile sisteme kimin giriş yaptığını kontrol edip basar.
- **`builtin_exit`:** Ana shell programını resmi olarak sonlandırır (`exit(0)`) ve C döngüsünü kapatır.

***

## 🏗️ 5. `Makefile` (Derleme Asistanı)

Tüm `.c` ve `.h` kod dosyalarını terminalden teker teker gcc compiler'a yazmak can sıkıcıdır. `Makefile`, tüm bu kod labirentindeki dosyaları tek tek bağımlılıklarına göre tarar, modern C11 standartları ve güvenlik için olan katı kuralları ( `-Wall`, `-Wpedantic` vb.) katarak bunları obje dosyalarına (`.o`) çevirip en sonunda da `myshell` ismindeki tek bir dosya haline (Linkleme) getirir.
