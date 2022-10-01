//
// Created by Olcay Taner YILDIZ on 5.01.2021.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <FsmMorphologicalAnalyzer.h>
#include "catch.hpp"
#include "../src/SimpleSpellChecker.h"

TEST_CASE("SimpleSpellCheckerTest-testSpellCheck") {
    FsmMorphologicalAnalyzer fsm = FsmMorphologicalAnalyzer();
    SimpleSpellChecker simpleSpellChecker = SimpleSpellChecker(fsm);
    Sentence* original[] = {new Sentence("yeni sezon başladı"),
                            new Sentence("sırtıkara adındaki canlı , bir balıktır"),
                            new Sentence("siyah ayı , ayıgiller familyasına ait bir ayı türüdür"),
                            new Sentence("yeni sezon başladı gibi"),
                            new Sentence("alışveriş için markete gitti"),
                            new Sentence("küçük bir yalıçapkını geçti"),
                            new Sentence("meslek odaları birliği yeniden toplandı"),
                            new Sentence("yeni yılın sonrasında vakalarda artış oldu"),
                            new Sentence("atomik saatin 10 mhz sinyali kalibrasyon hizmetlerinde referans olarak kullanılmaktadır"),
                            new Sentence("rehberimiz bu bölgedeki çıngıraklı yılan varlığı hakkında konuştu"),
                            new Sentence("bu son model cihaz 24 inç ekran büyüklüğünde ve 9 kg ağırlıktadır")};
    Sentence* modified[] = {new Sentence("yenisezon başladı"),
                            new Sentence("sırtı kara adındaki canlı , bir balıktır"),
                            new Sentence("siyahayı , ayıgiller familyasına ait bir ayı türüdür"),
                            new Sentence("yeni se zon başladı gibs"),
                            new Sentence("alis veriş için markete gitit"),
                            new Sentence("kucuk bri yalı çapkını gecti"),
                            new Sentence("mes lek odaları birliği yenidün toplandı"),
                            new Sentence("yeniyılın sonrasında vakalarda artış oldu"),
                            new Sentence("atomik saatin 10mhz sinyali kalibrasyon hizmetlerinde referans olarka kullanılmaktadır"),
                            new Sentence("rehperimiz buı bölgedeki çıngıraklıyılan varlıgı hakkınd konustu"),
                            new Sentence("bu son model ciha 24inç ekran büyüklüğünde ve 9kg ağırlıktadır")};
    for (int i = 0; i < 11; i++){
        REQUIRE(original[i]->to_string() == simpleSpellChecker.spellCheck(modified[i])->to_string());
    }
}