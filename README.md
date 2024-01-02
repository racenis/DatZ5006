# DatZ5006
 
## CEĻU MEKLĒŠANAS UN OPTIMIZĒŠANAS RĪKS

### Tehniskās informācijas

Lai nokompilētos vajag C++20 kompilatoru, varbūt vecākām versijām arī strādā.
Noteikti strādā ar gcc 12.1.0 MinGW.

Kompilācija:
```make```

Palaišana:
```prog input.txt output.txt 1000```

Tie input.txt, output.txt ir ievad/izvad faili. Beigās jāpieliek skaitlis, kas 
ierobežos algoritma izpildes laiku (algoritms ir ļoti lēns). Ja skaitlis ir
negatīvs, tad algoritms izmantos citu risinājumu ģenerēšanas metodi.

### Apraksts

Nezinu nevienu grafu īpašību, kas varētu atvieglot risinājuma izveidi.

Pirmā ideja bija tāda -- būs risinājumi, kas ir kopa ar šķaudnēm, uz kurām ir
podi. Vēl ir risinājumu ģenerators, kas ģenerē risinājumus. Tad ir risinājumu
pārbaudītājs, kas pārbauda vai ir spēkā noteikums ka katram iespējamajam ciklam
ir jābūt vismaz vienai appodotai šķaudnei. Beigās sanāk uzģenerēt kaut kādus
risinājumus, atrast derīgos un no tiem izvēlēties to, kuram ir labākā vērtība.

Vispirms uzrakstīju kodu priekš grafa ielādes un noparsēšanas no teksta uz
bināro formātu.

Pēc tam mēģināju visvienkāršāko iespējamo variantu, kā pārbaudīt vai visos
iespējamajos ciklos ir kāds pods -- no katras grafa virsotnes taisam pārbaudi,
kas izstaigās visus iespējamos ceļus kuri beidzās sākuma virsotnē, papildus
atceroties vai ceļā ir sastapts kāds pods.

Tad uzrakstīju risinājuma permutāciju ģeneratoru, kas vispirms uztaisīs
risinājumu ar tikai pirmo podu, tad tikai otro, tad pirmo un otro, tikai trešo,
utt.

Ģenerators ģenerē risinājumus, tad risinājumi tiek pārbaudīti. Atcerās risināumu
ar vislabāko novērtējumu.

Mēģināju palaist. Algoritms iestrēga eksponenciālajā līknē kaut kur pie 10-tās
šķaudnes. Sanāca 0 pareizi un 1023 nepareizi risinājumi. Izskatās ka nepareizo
risinājumu kopa ir daudz lielāka, nekā pareizo risināumu kopa. Labāka doma tādā
gadījumā varētu būt sākt no otra gala -- vispirms pieņemt ka visas šķaudnes ir
appodotas, tad katram risinājumam izmest ārā podus.

Pārmainīju kodu. Mēģināju palaist. Sanāca 15 pareizi un 16 nepareizi. Apnika
gaidīt, bet rezultāts noteikti labāks. Nepareizi risinājumi mūs neinteresē, un
šajā domēnā no nepareiza risinājuma nevar iegūt pareizu risinājumus, tā kā mēs
varētu vienkārši izslēgt no ģeneratora visus nepareizos risinājumus un to
atvasinājumus.

Pārrakstīju ģeneratoru lai tas no sākuma risinājuma izmet pa vienam podam,
pārbauda tos risinājumus, un ja tie ir pareizi, tad iemet pie sākuma risinājuma
un turpina ciklu. Pamēģināju palaist un atklājās ka mans risinājumu pārbaudes
kods ir šausmīgi lēns.

Pārrakstīju risinājumu pārbaudes kodu -- tagad tas apstaigās grafu un sameklēs
ciklus tikai vienu reizi un visus šos ciklus iespraudīs vienā lielā ciklu
sarakstā un tad pie katra risinājumu pārbaudes vienkārši izies cauri ciklu
sarakstam un pārbaudīs ka katrā ciklā ir vismaz viens pods.

Tagad algoritms tiek vismaz līdz 4525 risinājumu pārbaudes pirms man apnīk
gaidīt. Algoritms grafā atrod 131704 ciklus un tad katru risinājumu pret tiem
pārbauda, kas ir nedaudz par daudz. Ja mums ir grafs ar šķaudnēm ABC un tās
visas ir savienotas, tad algoritms atradīs ciklus ABC ACB BCA BAC CAB CBA, kas,
ja tā padomā, tad patiesībā ir tikai viens cikls, kas izliekas par vairākiem
cikliem (mums nav svarīga apstaigāšanas secība).

Sakārtojot katra cikla sastādošo šķaudņu sarakstu un tad sakārtojot visu ciklu
sarakstu un noņemot dublikātus man sanāca ciklu skaitu samazināt līdz 8454.
Tagad algoritms tiek līdz 31801 pārbaudītiem risinājumiem, līdz man apnīk gaidīt.

...

Tālāk nekas neizdevās un nesanāca, aizstāju risinājumu ģeneratoru ar nejaušu
risinājumu modificētāju un tad sanāca iegūt daudz labākus rezultātus, daudz
ātrāk. Joprojām nevar atrast pareizo risinājumu, algoritms nestrādā lielākiem
grafiem, nekas neizdodas, es padodos.

### Novērtējums

Ciklu meklēšanas algoritms pie katras virsotnes sāks meklēšanu, kuras laikā tas 
veiks meklēšanu plašumā, mēģinot atrast ceļus uz sākuma virsotni. Tas sanāk ka 
ir v+e laiks, mums v=n un sliktākajā gadījumā e=n^2, vēl klāt nāk rekursija, kas 
iedos zem n^2 laiku. Kopā ciklu meklēsana sanāks n*(n^2*(n+n^2)), jeb n*(n^3+n^4), 
jeb n^4+n^5.

Risinājuma pārbaudes laiks sliktākajā gadījumā ir n^2 * ciklu skaits, kas šeit 
būs n^2*(n^4+n^5), jeb n^6 + n^7.

Papildus tiek izmantoti daudz std::set, kas gcc implementācijā aizņems log(n) 
laiku gan iespraušanai, gan meklēšanai. Tam kopējo laiku nevajadzētu baigi ietekmēt.

Es nezinu cik daudz laika aizņems paša risinājumu ģenerācija, bet man liekas ka 
tas varētu būt nedaudz pailgi, bet tā kā algoritms iterējās pāri tikai derīgajiem 
risinājumiem un met ārā nederīgos, tam vajadzētu būt zem eksponenciālā laika.

Pa lielam mans algoritms doto problēmu (jābūt vienam podam katrā ceļā) pārveidot 
par SAT problēmu un tad meklē tai risinājumus (jābūt mazākajai iespējamajai podu 
vērtībai) un vēl tos optimizē, kas, manuprāt, nebija tā pati labākā doma, bet es 
nezinu labāku veidu un es jau 3 dienas cenšos pabeigt šo uzdevumu un es neko 
labāku nevaru izdomāt.

### Pašnovērtējums

Ļoti slikti! Nekas nesanāk!

