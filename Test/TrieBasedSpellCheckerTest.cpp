//
// Created by Olcay Taner YILDIZ on 9.03.2023.
//

#include <FsmMorphologicalAnalyzer.h>
#include <NoSmoothing.h>
#include "catch.hpp"
#include "../src/TrieBasedSpellChecker.h"

TEST_CASE("TrieBasedSpellCheckerTest-testSpellCheck") {
    FsmMorphologicalAnalyzer fsm = FsmMorphologicalAnalyzer();
    auto* nGram = new NGram<string>("ngram.txt");
    NoSmoothing<string> noSmoothing;
    noSmoothing.setProbabilities(*nGram);
    Sentence* original[] = {                new Sentence("antibiyotik direnci bir mikroorganizmanın antibiyotiklerin etkilerine karşı durabilme yeteneğidir"),
                                            new Sentence("bugünkü ortaöğretim sisteminin oluşumu Cumhuriyet döneminde gerçekleşmiştir"),
                                            new Sentence("bilinçsizce dağıtılan kredilerin sorun yaratmayacağını düşünmelerinin nedeni bankaların büyüklüğünden kaynaklanıyordu"),
                                            new Sentence("Yemen'de ekonomik durgunluk nedeniyle yeni bir insani kriz yaşanabileceği uyarısı yapıldı"),
                                            new Sentence("hafif ticari araç bariyerlere çarptı"),
                                            new Sentence("sırtıkara adındaki canlı , bir balıktır"),
                                            new Sentence("siyah ayı , ayıgiller familyasına ait bir ayı türüdür"),
                                            new Sentence("alışveriş için markete gitti"),
                                            new Sentence("küçük bir yalıçapkını geçti"),
                                            new Sentence("yeni yılın sonrasında vakalarda artış oldu"),
                                            new Sentence("atomik saatin 10 mhz sinyali kalibrasyon hizmetlerinde referans olarak kullanılmaktadır"),
                                            new Sentence("rehberimiz bu bölgedeki çıngıraklı yılan varlığı hakkında konuştu"),
                                            new Sentence("bu haksızlık da unutulup gitmişti"),
                                            new Sentence("4'lü tahıl zirvesi İstanbul'da gerçekleşti"),
                                            new Sentence("10'luk sistemden 100'lük sisteme geçiş yapılacak"),
                                            new Sentence("play-off maçlarına çıkacak takımlar belli oldu"),
                                            new Sentence("bu son model cihaz 24 inç ekran büyüklüğünde ve 9 kg ağırlıktadır")};
    Sentence* modified[] = {                new Sentence("antibiodik direnci bir mikrorkanismanın antibiyotiklerin etkilerine karşı durabilme yeteneğidir"),
                                            new Sentence("bugünki ortögretim sisteminin oluşumu Cumhuriyet dönmeinde gerçekleşmiştir"),
                                            new Sentence("billinçiszce dağıtılan kredilerin sorun yaratmayacağını düşünmelerinin nedeni bankaların büyüklüğünden kaynaklanıyordu"),
                                            new Sentence("Yemen'de ekonomik durrğunlk nedeniyle yeni bir insani kriz yaşanabileceği uyaarisi yapıldı"),
                                            new Sentence("hafif ricarii araç aryerlere çarptı"),
                                            new Sentence("sırtı kara adındaki canlı , bir balıktır"),
                                            new Sentence("siyahayı , ayıgiller familyasına ait bir ayı türüdür"),
                                            new Sentence("alis veriş için markete gitit"),
                                            new Sentence("kucuk bri yalı çapkını gecti"),
                                            new Sentence("yeniyılın sonrasında vakalarda artış oldu"),
                                            new Sentence("atomik saatin 10mhz sinyali kalibrasyon hizmetlerinde referans olarka kullanılmaktadır"),
                                            new Sentence("rehperimiz buı bölgedeki çıngıraklıyılan varlıgı hakkınd konustu"),
                                            new Sentence("bu haksızlıkda unutulup gitmişti"),
                                            new Sentence("4 lı tahıl zirvesi İstanbul'da gerçekleşti"),
                                            new Sentence("10 lük sistemden 100 lık sisteme geçiş yapılacak"),
                                            new Sentence("play - off maçlarına çıkacak takımlar belli oldu"),
                                            new Sentence("bu son model ciha 24inç ekran büyüklüğünde ve 9kg ağırlıktadır")};
    TrieBasedSpellChecker trieBasedSpellChecker = TrieBasedSpellChecker(fsm, nGram, SpellCheckerParameter());
    for (int i = 0; i < 17; i++){
        REQUIRE(original[i]->to_string() == trieBasedSpellChecker.spellCheck(modified[i])->to_string());
    }
}
