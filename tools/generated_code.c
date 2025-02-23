#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct wordDictEntry {
    const char* word;
    int hex_val;
};

const struct wordDictEntry wordDict[] = {
    {"homes", 0x00},
    {"seasons", 0x01},
    {"vehicles", 0x02},
    {"scale", 0x03},
    {"workplace", 0x04},
    {"monster", 0x05},
    {"fourth", 0x06},
    {"sbjct", 0x07},
    {"texture", 0x08},
    {"amy", 0x09},
    {"stood", 0x0a},
    {"planner", 0x0b},
    {"documentary", 0x0c},
    {"friday", 0x0d},
    {"grave", 0x0e},
    {"softball", 0x0f},
    {"handy", 0x10},
    {"infection", 0x11},
    {"pioneer", 0x12},
    {"opponents", 0x13},
    {"awful", 0x14},
    {"advance", 0x15},
    {"competition", 0x16},
    {"banks", 0x17},
    {"technique", 0x18},
    {"biological", 0x19},
    {"licensing", 0x1a},
    {"occur", 0x1b},
    {"tuner", 0x1c},
    {"savings", 0x1d},
    {"judy", 0x1e},
    {"newfoundland", 0x1f},
    {"india", 0x20},
    {"fcc", 0x21},
    {"nutrition", 0x22},
    {"commands", 0x23},
    {"etc", 0x24},
    {"advanced", 0x25},
    {"photographs", 0x26},
    {"zip", 0x27},
    {"attractions", 0x28},
    {"conflict", 0x29},
    {"grade", 0x2a},
    {"sheet", 0x2b},
    {"jan", 0x2c},
    {"villas", 0x2d},
    {"interval", 0x2e},
    {"rj", 0x2f},
    {"groups", 0x30},
    {"yn", 0x31},
    {"siemens", 0x32},
    {"alpine", 0x33},
    {"contains", 0x34},
    {"companies", 0x35},
    {"heated", 0x36},
    {"vpn", 0x37},
    {"zu", 0x38},
    {"sullivan", 0x39},
    {"boolean", 0x3a},
    {"asked", 0x3b},
    {"rentcom", 0x3c},
    {"project", 0x3d},
    {"ist", 0x3e},
    {"fifteen", 0x3f},
    {"packard", 0x40},
    {"rangers", 0x41},
    {"kiss", 0x42},
    {"addressed", 0x43},
    {"little", 0x44},
    {"assignment", 0x45},
    {"wellness", 0x46},
    {"wood", 0x47},
    {"lady", 0x48},
    {"somerset", 0x49},
    {"dc", 0x4a},
    {"wiring", 0x4b},
    {"screensaver", 0x4c},
    {"acne", 0x4d},
    {"items", 0x4e},
    {"screenshot", 0x4f},
    {"song", 0x50},
    {"fortune", 0x51},
    {"book", 0x52},
    {"virtue", 0x53},
    {"brakes", 0x54},
    {"concerts", 0x55},
    {"surgeon", 0x56},
    {"conducted", 0x57},
    {"calcium", 0x58},
    {"vip", 0x59},
    {"environmental", 0x5a},
    {"wells", 0x5b},
    {"resident", 0x5c},
    {"upset", 0x5d},
    {"deaths", 0x5e},
    {"affordable", 0x5f},
    {"reject", 0x60},
    {"paste", 0x61},
    {"raid", 0x62},
    {"ashley", 0x63},
    {"detected", 0x64},
    {"licking", 0x65},
    {"managers", 0x66},
    {"belize", 0x67},
    {"unwrap", 0x68},
    {"sync", 0x69},
    {"downloadcom", 0x6a},
    {"incurred", 0x6b},
    {"hero", 0x6c},
    {"head", 0x6d},
    {"appropriate", 0x6e},
    {"actively", 0x6f},
    {"container", 0x70},
    {"affected", 0x71},
    {"smell", 0x72},
    {"weapon", 0x73},
    {"louisiana", 0x74},
    {"yeast", 0x75},
    {"defining", 0x76},
    {"consists", 0x77},
    {"stock", 0x78},
    {"mistress", 0x79},
    {"antibodies", 0x7a},
    {"outlined", 0x7b},
    {"your", 0x7c},
    {"marion", 0x7d},
    {"communication", 0x7e},
    {"saved", 0x7f},
    {"functioning", 0x80},
    {"missions", 0x81},
    {"growing", 0x82},
    {"snapshot", 0x83},
    {"starts", 0x84},
    {"washing", 0x85},
    {"affiliated", 0x86},
    {"mortgage", 0x87},
    {"adware", 0x88},
    {"municipal", 0x89},
    {"about", 0x8a},
    {"hidden", 0x8b},
    {"brian", 0x8c},
    {"deny", 0x8d},
    {"attract", 0x8e},
    {"publicity", 0x8f},
    {"distant", 0x90},
    {"supervisor", 0x91},
    {"employer", 0x92},
    {"featured", 0x93},
    {"news", 0x94},
    {"atlantic", 0x95},
    {"requires", 0x96},
    {"algebra", 0x97},
    {"element", 0x98},
    {"notified", 0x99},
    {"floyd", 0x9a},
    {"empirical", 0x9b},
    {"gazette", 0x9c},
    {"bernard", 0x9d},
    {"nudist", 0x9e},
    {"favor", 0x9f},
    {"est", 0xa0},
    {"forest", 0xa1},
    {"strike", 0xa2},
    {"asbestos", 0xa3},
    {"ht", 0xa4},
    {"hazardous", 0xa5},
    {"renewal", 0xa6},
    {"harassment", 0xa7},
    {"scanning", 0xa8},
    {"instruction", 0xa9},
    {"planned", 0xaa},
    {"curious", 0xab},
    {"accordingly", 0xac},
    {"dylan", 0xad},
    {"screening", 0xae},
    {"preparing", 0xaf},
    {"munich", 0xb0},
    {"showed", 0xb1},
    {"instrumentation", 0xb2},
    {"punk", 0xb3},
    {"bedford", 0xb4},
    {"funky", 0xb5},
    {"analyzed", 0xb6},
    {"wilderness", 0xb7},
    {"playstation", 0xb8},
    {"damages", 0xb9},
    {"carol", 0xba},
    {"dressed", 0xbb},
    {"spyware", 0xbc},
    {"unlike", 0xbd},
    {"degree", 0xbe},
    {"effectiveness", 0xbf},
    {"beyond", 0xc0},
    {"optimize", 0xc1},
    {"fox", 0xc2},
    {"acknowledged", 0xc3},
    {"gas", 0xc4},
    {"motor", 0xc5},
    {"chairs", 0xc6},
    {"refused", 0xc7},
    {"evans", 0xc8},
    {"rubber", 0xc9},
    {"abortion", 0xca},
    {"niger", 0xcb},
    {"xnxx", 0xcc},
    {"gamespot", 0xcd},
    {"blacks", 0xce},
    {"harry", 0xcf},
    {"baseline", 0xd0},
    {"dvds", 0xd1},
    {"rich", 0xd2},
    {"apt", 0xd3},
    {"claims", 0xd4},
    {"executive", 0xd5},
    {"short", 0xd6},
    {"offerings", 0xd7},
    {"few", 0xd8},
    {"advocacy", 0xd9},
    {"masturbation", 0xda},
    {"downtown", 0xdb},
    {"promise", 0xdc},
    {"neon", 0xdd},
    {"mats", 0xde},
    {"socks", 0xdf},
    {"silent", 0xe0},
    {"invited", 0xe1},
    {"brain", 0xe2},
    {"measurements", 0xe3},
    {"pain", 0xe4},
    {"hood", 0xe5},
    {"every", 0xe6},
    {"communist", 0xe7},
    {"diesel", 0xe8},
    {"proportion", 0xe9},
    {"having", 0xea},
    {"allocation", 0xeb},
    {"associate", 0xec},
    {"treo", 0xed},
    {"stick", 0xee},
    {"feedback", 0xef},
    {"segment", 0xf0},
    {"percent", 0xf1},
    {"buildings", 0xf2},
    {"earnings", 0xf3},
    {"sentence", 0xf4},
    {"this", 0xf5},
    {"usps", 0xf6},
    {"subsection", 0xf7},
    {"divided", 0xf8},
    {"obligations", 0xf9},
    {"zus", 0xfa},
    {"maine", 0xfb},
    {"fool", 0xfc},
    {"practitioners", 0xfd},
    {"easier", 0xfe},
    {"parking", 0xff}
};


const char* data1 = "asked villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas villas screenshot book somerset wood somerset items rangers screensaver villas villas villas villas villas villas villas villas villas villas friday stood friday stood friday ";
const char* data2 = "stood affordable weapon louisiana paste smell louisiana boolean friday stood india india india india ashley hero detected friday stood india india india india paste appropriate detected india smell weapon container jan groups stock managers managers managers managers managers managers managers managers managers managers managers managers managers managers managers groups friday ";
const char* data3 = "stood india india india india ashley paste hero hero india weapon paste defining licking affordable raid licking belize sync appropriate appropriate sync appropriate belize friday stood friday stood head licking louisiana paste weapon container hero actively sync louisiana affordable raid hero actively ashley incurred affordable paste container sync boolean friday ";
const char* data4 = "stood india india india india container yeast weapon unwrap india smell sullivan friday stood india india india india container yeast weapon unwrap india smell zu friday stood india india india india stock actively smell india smell paste stock jan smell paste stock friday stood india india india india head actively ";
const char* data5 = "defining india smell paste stock jan groups stock managers managers friday stood india india india india container yeast weapon unwrap india smell detected stock friday stood india india india india stock actively smell india smell detected stock jan smell detected stock friday stood india india india india stock actively smell ";
const char* data6 = "india smell paste stock jan groups stock sullivan managers friday stood india india india india head actively defining india smell paste stock jan wells belize weapon boolean smell paste stock upset india asked india groups stock heated groups friday stood india india india india stock actively smell india smell paste ";
const char* data7 = "stock jan advanced book rangers items little screenshot acne affordable yn advanced friday stood india india india india container yeast weapon unwrap india smell ashley stock friday stood india india india india head actively defining india smell detected stock jan smell paste stock friday stood india india india india stock ";
const char* data8 = "actively smell india smell detected stock jan advanced book rangers items little screenshot acne affordable yn advanced friday stood india india india india asked india head actively defining india smell detected stock jan wells smell paste stock sheet groups stock heated groups upset friday stood india india india india head ";
const char* data9 = "actively defining india smell detected stock jan wells smell detected stock sheet groups stock yn zu upset friday stood india india india india stock actively smell india smell detected stock jan smell paste stock friday stood india india india india container yeast weapon unwrap india smell weapon sync friday stood ";
const char* data10 = "india india india india stock actively smell india smell detected stock jan smell paste stock friday stood india india india india appropriate actively container friday stood india india india india head actively defining india smell detected stock jan wells smell detected stock sheet groups stock siemens groups upset friday stood ";
const char* data11 = "friday stood hero actively paste detected affordable head actively detected yeast hero licking affordable appropriate paste head licking boolean friday stood india india india india head actively defining india smell weapon sync jan wells smell detected stock sheet groups stock companies groups upset india india india india india friday stood ";
const char* data12 = "india india india india head actively defining antibodies stock india smell ashley stock jan india consists actively smell detected india wells smell detected stock sheet groups stock contains paste upset friday stood india india india india stock actively smell india smell sullivan jan smell sullivan india india friday stood india ";
const char* data13 = "india india india weapon unwrap hero india smell ashley stock jan india yn friday stood friday stood head actively detected yeast hero licking affordable unwrap paste weapon unwrap affordable hero actively actively container boolean friday stood india india india india stock actively smell india smell paste stock jan smell paste ";
const char* data14 = "stock india friday stood india india india india hero actively detected weapon raid india friday stood india india india india ashley head container india paste hero jan groups stock heated yn friday stood india india india india downloadcom hero india weapon incurred sync container affordable ashley paste weapon licking affordable ";
const char* data15 = "paste detected downloadcom yeast weapon louisiana head licking appropriate louisiana friday stood india india india india weapon yeast raid india paste hero jan groups stock siemens groups friday stood weapon incurred sync container affordable ashley paste weapon licking affordable paste detected downloadcom yeast weapon louisiana head licking appropriate louisiana boolean ";
const char* data16 = "friday stood india india india india smell actively smell india smell sullivan detected jan advanced book screenshot book affordable surgeon rangers screensaver concerts assignment advanced friday stood india india india india detected licking ashley india smell ashley stock friday stood india india india india paste detected detected india smell sullivan ";
const char* data17 = "detected jan licking paste stock friday stood india india india india hero actively actively container india head actively detected yeast hero licking affordable unwrap paste weapon unwrap affordable hero actively actively container friday stood friday stood sync weapon affordable defining paste hero sync detected affordable head actively detected yeast hero ";
const char* data18 = "licking boolean friday stood india india india india container yeast weapon unwrap india smell detected stock friday stood india india india india container yeast weapon unwrap india smell sullivan friday stood india india india india head actively defining india smell detected stock jan wells smell detected stock sheet groups stock ";
const char* data19 = "siemens groups upset india india india asked india friday stood india india india india head actively defining india licking paste stock jan wells smell detected stock sheet groups stock alpine ashley upset india india india asked india actively managers managers weapon licking louisiana india detected licking india hero zip licking ";
const char* data20 = "appropriate louisiana licking louisiana licking india song assignment india attractions licking affordable hero managers paste appropriate licking consists conflict friday stood india india india india paste detected detected india smell paste stock jan smell detected stock india india india india india india india india india india asked india book rangers ";
const char* data21 = "calcium india weapon sync louisiana yeast acknowledged instruction india acknowledged est india hero zip licking appropriate louisiana licking louisiana licking india song assignment friday stood india india india india ashley head container india consists actively smell detected india wells smell paste stock sheet groups stock yn zu upset jan groups ";
const char* data22 = "stock siemens groups raid india india india asked india surgeon acknowledged instruction smell sync managers sync licking smell india weapon sync india ashley zip licking weapon louisiana india yeast appropriate licking india sync head paste belize licking india heated contains india raid sync louisiana weapon india attractions groups stock siemens ";
const char* data23 = "groups raid india ashley actively smell smell licking weapon container actively appropriate detected india acknowledged est india song assignment heated contains conflict friday stood india india india india downloadcom appropriate antibodies india hero actively paste detected affordable appropriate licking stock louisiana affordable head actively detected yeast hero licking india india ";
const char* data24 = "india india india india india india asked india virtue sync india container paste weapon india hero licking india ashley paste weapon jan india actively appropriate india container paste weapon weapon licking india paste yeast india head actively detected yeast hero licking india weapon yeast sync defining paste appropriate louisiana friday ";
const char* data25 = "stood friday stood india india india india head actively defining india licking paste stock jan wells smell paste stock sheet groups stock zu zu upset india india india asked india actively managers managers weapon licking louisiana india detected licking india somerset acne rangers wood assignment affordable assignment calcium song screenshot ";
const char* data26 = "book brakes affordable little somerset book assignment addressed brakes screenshot book vip friday stood india india india india louisiana licking weapon louisiana india smell paste stock jan smell paste stock india india india india india india india india india asked india weapon sync india container paste weapon india detected zip ";
const char* data27 = "actively managers managers weapon licking louisiana india container actively yeast smell india somerset acne rangers wood assignment affordable assignment calcium song screenshot book brakes affordable little somerset book assignment addressed brakes screenshot book vip jan india hero licking india head actively detected yeast hero licking india appropriate zip licking weapon ";
const char* data28 = "louisiana india container paste weapon india hero licking india raid actively appropriate jan india actively appropriate india container paste weapon weapon licking india paste yeast india weapon yeast sync defining paste appropriate louisiana friday stood india india india india downloadcom antibodies india hero actively paste detected affordable appropriate licking stock ";
const char* data29 = "louisiana affordable head actively detected yeast hero licking friday stood friday stood india india india india paste detected detected india smell paste stock jan smell detected stock india india india india india india india india india india asked india screenshot appropriate india smell licking ashley yeast container licking smell licking ";
const char* data30 = "india hero zip paste detected smell licking weapon weapon licking india detected licking india somerset acne rangers wood assignment affordable assignment calcium song screenshot book brakes affordable little somerset book assignment addressed brakes screenshot book vip jan india licking hero hero licking india ashley actively appropriate louisiana sync licking appropriate ";
const char* data31 = "louisiana india detected licking weapon india paste detected smell licking weapon weapon licking weapon india smell licking hero paste louisiana sync defining licking weapon india attractions actively managers managers weapon licking louisiana weapon conflict india detected licking india louisiana paste raid hero licking weapon india detected actively appropriate louisiana india ";
const char* data32 = "appropriate actively yeast weapon india paste hero hero actively appropriate weapon india paste defining actively sync smell india raid licking weapon actively sync appropriate friday stood india india india india head actively defining india licking ashley stock jan wells smell paste stock sheet groups stock yn zu upset india india ";
const char* data33 = "india asked india items yeast head raid licking smell screenshot managers items paste head licking weapon friday stood india india india india head actively defining india smell zu detected jan wells smell paste stock sheet groups stock siemens groups upset india india india asked india rangers detected detected smell licking ";
const char* data34 = "weapon weapon screenshot managers items paste head licking weapon india attractions actively managers managers weapon licking louisiana india detected licking india hero paste india louisiana paste raid hero licking india detected licking weapon india book surgeon rangers india detected licking weapon india appropriate actively head weapon india detected licking weapon ";
const char* data35 = "india managers actively appropriate ashley louisiana sync actively appropriate weapon conflict friday stood india india india india paste detected detected india smell zu jan smell detected stock friday stood india india india india container yeast weapon unwrap india smell paste stock india india india india india india india india india ";
const char* data36 = "india india india india asked india screenshot appropriate india weapon paste yeast defining licking belize paste smell detected licking india hero zip paste detected smell licking weapon weapon licking india detected licking india somerset acne rangers wood assignment affordable assignment calcium song screenshot book brakes affordable little somerset book assignment ";
const char* data37 = "addressed brakes screenshot book vip friday stood friday stood managers sync appropriate detected affordable managers yeast appropriate ashley louisiana sync actively appropriate boolean friday stood india india india india downloadcom smell ashley stock antibodies india container smell licking affordable hero actively paste detected affordable appropriate licking stock louisiana affordable head ";
const char* data38 = "actively detected yeast hero licking friday stood india india india india stock actively smell india smell sullivan jan smell sullivan friday stood india india india india detected licking ashley india smell ashley stock friday stood india india india india head actively defining india licking weapon sync jan wells smell zu ";
const char* data39 = "sheet smell ashley stock grade contains upset india friday stood india india india india paste detected detected india smell weapon sync jan smell detected stock friday stood friday stood managers yeast appropriate ashley louisiana sync actively appropriate affordable unwrap paste weapon unwrap affordable hero actively actively container boolean friday stood ";
const char* data40 = "india india india india stock actively smell india smell paste stock jan smell paste stock friday stood india india india india hero actively detected weapon raid friday stood india india india india smell actively smell india smell sullivan detected jan advanced book screenshot book affordable surgeon rangers screensaver concerts assignment ";
const char* data41 = "advanced friday stood india india india india paste detected detected india smell sullivan detected jan licking paste stock friday stood india india india india ashley head container india paste hero jan paste unwrap friday stood india india india india downloadcom appropriate antibodies india managers yeast appropriate ashley louisiana sync actively ";
const char* data42 = "appropriate affordable unwrap paste weapon unwrap affordable hero actively actively container friday stood friday stood ashley unwrap licking ashley incurred boolean friday stood india india india india paste detected detected india smell sullivan jan wells smell weapon container sheet groups stock zu upset india india india india asked india actively ";
const char* data43 = "appropriate india paste actively yeast louisiana licking india paste yeast india unwrap paste weapon unwrap india ashley licking hero yeast sync india detected yeast india head actively detected yeast hero licking friday stood india india india india ashley head container india smell sullivan detected jan smell yn groups detected india ";
const char* data44 = "india india india india india india india asked india paste managers sync appropriate india detected zip licking weapon weapon paste mistress licking smell india detected licking india louisiana smell actively yeast defining licking smell india ashley licking hero yeast sync india weapon louisiana actively ashley incurred acknowledged instruction india licking ";
const char* data45 = "appropriate india detected yeast smell friday stood india india india india downloadcom appropriate antibodies india managers sync appropriate detected affordable managers yeast appropriate ashley louisiana sync actively appropriate india india india asked india virtue sync india yeast appropriate licking india ashley actively smell smell licking weapon container actively appropriate detected ";
const char* data46 = "paste appropriate ashley licking india paste india acknowledged instruction louisiana acknowledged instruction india louisiana smell actively yeast defining acknowledged instruction licking jan india actively appropriate india container paste weapon weapon licking india acknowledged est india hero paste india weapon yeast sync louisiana licking friday stood managers sync appropriate paste hero ";
const char* data47 = "boolean india india india india india india india india india india india india india india india india india india friday stood india india india india container actively container india smell paste stock india india india india india india india india india india india india india asked india screenshot appropriate india ";
const char* data48 = "smell licking ashley yeast container licking smell licking india paste detected smell licking weapon weapon licking india detected licking india somerset acne rangers wood assignment affordable assignment calcium song screenshot book brakes affordable little somerset book assignment addressed brakes screenshot book vip friday stood india india india india head actively ";
const char* data49 = "defining india smell zu detected jan wells smell paste stock sheet groups stock siemens contains upset india india asked india rangers detected detected smell licking weapon weapon screenshot managers items paste head licking screenshot smell detected sync appropriate paste hero weapon jan india acknowledged est india somerset acne rangers wood ";
const char* data50 = "assignment affordable assignment calcium song screenshot book brakes affordable little somerset book assignment addressed brakes screenshot book vip india sheet india groups stock siemens contains friday stood india india india india paste detected detected india smell zu jan smell detected stock friday stood india india india india head actively defining ";
const char* data51 = "india ashley stock jan wells smell zu sheet smell ashley stock grade siemens upset india india india asked india smell ashley stock india licking weapon louisiana india hero zip sync appropriate detected licking stock india detected licking india hero paste india managers actively appropriate ashley louisiana sync actively appropriate india ";
const char* data52 = "louisiana smell actively yeast defining acknowledged instruction licking india container smell licking ashley licking detected licking head licking appropriate louisiana india detected paste appropriate weapon india rangers detected detected smell licking weapon weapon screenshot managers items paste head licking weapon jan india actively appropriate india smell licking ashley yeast container ";
const char* data53 = "licking smell licking india hero paste india defining paste hero licking yeast smell india ashley actively smell smell licking weapon container actively appropriate detected paste appropriate louisiana licking india detected paste appropriate weapon india rangers detected detected smell licking weapon weapon screenshot managers items paste head licking screenshot smell detected ";
const char* data54 = "sync appropriate paste hero weapon friday stood india india india india head actively defining india smell zu detected jan wells smell paste stock sheet groups stock yn ashley upset india india asked india assignment rangers brakes jan india acknowledged est india somerset acne rangers wood assignment affordable assignment calcium song ";
const char* data55 = "screenshot book brakes affordable little somerset book assignment addressed brakes screenshot book vip india sheet india groups stock yn addressed india attractions paste yeast weapon weapon sync india paste container container licking hero acknowledged instruction india rangers detected detected smell licking weapon weapon screenshot managers items paste head licking weapon ";
const char* data56 = "conflict friday stood india india india india paste detected detected india smell zu jan smell detected stock india india india friday stood india india india india head actively defining india licking paste stock jan wells smell zu sheet smell ashley stock grade contains upset india friday stood india india india ";
const char* data57 = "india container actively container india smell zu friday stood india india india india paste detected detected india smell paste stock jan smell detected stock india friday stood india india india india stock actively smell india smell paste stock jan advanced book rangers items little screenshot acne affordable siemens advanced friday ";
const char* data58 = "stood india india india india container actively container india smell zu friday stood india india india india container actively container india smell weapon sync friday stood india india india india container actively container india smell ashley stock india friday stood india india india india container actively container india smell detected ";
const char* data59 = "stock india india friday stood india india india india container actively container india smell zu india friday stood india india india india container actively container india smell sullivan india friday stood india india india india container actively container india smell yn groups friday stood india india india india weapon yeast ";
const char* data60 = "raid india smell weapon container jan raid mistress louisiana licking india groups stock siemens groups friday stood india india india india container yeast weapon unwrap india smell yn groups friday stood india india india india stock actively smell india smell paste stock jan advanced book rangers items little screenshot acne ";
const char* data61 = "affordable siemens advanced friday stood india india india india downloadcom head container india smell paste stock india friday stood friday stood container smell licking affordable hero actively paste detected affordable appropriate licking stock louisiana affordable head actively detected yeast hero licking boolean friday stood india india india india container actively ";
const char* data62 = "container india smell paste stock india india india india india india india india india india india india india friday stood hero actively paste detected affordable appropriate licking stock louisiana affordable head actively detected yeast hero licking boolean friday stood india india india india container actively container india smell sullivan friday ";
const char* data63 = "stood india india india india container actively container india smell detected stock friday stood india india india india head actively defining india smell detected stock jan india wells smell detected stock upset friday stood india india india india downloadcom head container india hero actively paste detected affordable head actively detected ";
const char* data64 = "yeast hero licking affordable appropriate paste head licking friday stood friday stood weapon paste defining licking affordable raid licking belize sync appropriate appropriate sync appropriate belize boolean friday stood india india india india container actively container india smell raid container india india india india india india india india india india ";
const char* data65 = "india india india india india asked india smell raid container india project india packard villas ist raid licking belize sync appropriate appropriate sync appropriate belize india attractions head licking louisiana paste weapon container hero actively sync louisiana affordable raid hero actively ashley incurred affordable paste container sync conflict friday stood ";
const char* data66 = "india india india india ashley paste hero hero india hero actively paste detected affordable concerts virtue assignment book alpine siemens little screensaver screensaver friday stood india india india india detected raid india reject yeast weapon licking smell alpine siemens interval detected hero hero resident groups reject friday stood friday stood ";
const char* data67 = "hero actively paste detected affordable concerts virtue assignment book alpine siemens little screensaver screensaver boolean friday stood india india india india container actively container india smell ashley stock india india india friday stood india india india india head actively defining india smell yn groups detected jan advanced lady rangers virtue ";
const char* data68 = "lady affordable incurred licking smell appropriate licking hero alpine siemens interval detected hero hero affordable screensaver actively paste detected screensaver sync raid smell paste smell mistress rangers advanced india india india asked india lady paste weapon unwrap india detected licking india incurred licking smell appropriate licking hero alpine siemens interval ";
const char* data69 = "screensaver actively paste detected screensaver sync raid smell paste smell mistress rangers india friday stood india india india india ashley paste hero hero india smell raid container india friday stood india india india india head actively defining india smell sullivan jan india groups stock groups india india india india india ";
const char* data70 = "friday stood india india india india ashley paste hero hero india hero actively paste detected affordable lady assignment screensaver screensaver screenshot friday stood india india india india detected raid india reject lady licking hero hero actively resident groups reject friday stood friday stood hero actively paste detected affordable lady assignment ";
const char* data71 = "screensaver screensaver screenshot boolean friday stood india india india india container actively container india smell detected stock friday stood india india india india ashley paste hero hero india hero actively paste detected affordable somerset items wellness screenshot friday stood india india india india detected raid india reject somerset appropriate managers ";
const char* data72 = "actively resident groups reject friday stood friday stood hero actively paste detected affordable somerset items wellness screenshot boolean friday stood india india india india container actively container india smell zu friday stood friday stood hero actively paste detected affordable unwrap paste weapon unwrap licking weapon boolean friday stood india india ";
const char* data73 = "india india stock actively smell india smell ashley stock jan smell ashley stock friday stood india india india india head actively defining india smell yn groups detected jan advanced lady rangers virtue lady affordable yeast weapon licking smell alpine siemens interval detected hero hero affordable acne licking weapon weapon paste ";
const char* data74 = "belize licking kiss actively stock rangers advanced india india india india asked india lady paste weapon unwrap india detected licking india yeast weapon licking smell alpine siemens interval acne licking weapon weapon paste belize licking kiss actively stock rangers friday stood india india india india ashley paste hero hero india ";
const char* data75 = "smell raid container friday stood india india india india stock actively smell india smell ashley stock jan smell ashley stock friday stood india india india india head actively defining india smell yn groups detected jan advanced lady rangers virtue lady affordable incurred licking smell appropriate licking hero alpine siemens interval ";
const char* data76 = "detected hero hero affordable assignment stock sync louisiana song smell actively ashley licking weapon weapon advanced india india india asked india lady paste weapon unwrap india detected licking india incurred licking smell appropriate licking hero alpine siemens interval assignment stock sync louisiana song smell actively ashley licking weapon weapon friday ";
const char* data77 = "stood india india india india ashley paste hero hero india smell raid container ";

size_t total_size = strlen(data1) + strlen(data2) + strlen(data3) + strlen(data4) + strlen(data5) + strlen(data6) + strlen(data7) + strlen(data8) + strlen(data9) + strlen(data10) + strlen(data11) + strlen(data12) + strlen(data13) + strlen(data14) + strlen(data15) + strlen(data16) + strlen(data17) + strlen(data18) + strlen(data19) + strlen(data20) + strlen(data21) + strlen(data22) + strlen(data23) + strlen(data24) + strlen(data25) + strlen(data26) + strlen(data27) + strlen(data28) + strlen(data29) + strlen(data30) + strlen(data31) + strlen(data32) + strlen(data33) + strlen(data34) + strlen(data35) + strlen(data36) + strlen(data37) + strlen(data38) + strlen(data39) + strlen(data40) + strlen(data41) + strlen(data42) + strlen(data43) + strlen(data44) + strlen(data45) + strlen(data46) + strlen(data47) + strlen(data48) + strlen(data49) + strlen(data50) + strlen(data51) + strlen(data52) + strlen(data53) + strlen(data54) + strlen(data55) + strlen(data56) + strlen(data57) + strlen(data58) + strlen(data59) + strlen(data60) + strlen(data61) + strlen(data62) + strlen(data63) + strlen(data64) + strlen(data65) + strlen(data66) + strlen(data67) + strlen(data68) + strlen(data69) + strlen(data70) + strlen(data71) + strlen(data72) + strlen(data73) + strlen(data74) + strlen(data75) + strlen(data76) + strlen(data77);

const char* obf_data = (char*)malloc(total_size + 1);
strcpy(obf_data, data1);
strcat(obf_data, data2);
strcat(obf_data, data3);
strcat(obf_data, data4);
strcat(obf_data, data5);
strcat(obf_data, data6);

char* decodePayload(const char* encoded) {
    char* decoded = (char*)malloc(strlen(encoded) + 1);
    char* p = decoded;
    const char* start = encoded;
    while (*start) {
        const char* end = start;
        while (*end && *end != ' ') {
            ++end;
        }

        size_t len = end - start;
        char word[len + 1];
        strncpy(word, start, len);
        word[len] = '\0';

        int found = 0;
        for (int i = 0; i < sizeof(wordDict) / sizeof(wordDict[0]); ++i) {
            if (strcmp(wordDict[i].word, word) == 0) {
                *p = (char)strtol(wordDict[i].hex_val + 2, NULL, 16);
                ++p;
                found = 1;
                break;
            }
        }

        if (!found) {
            fprintf(stderr, "[!] Erreur de décodage : mot inconnu %s\n", word);
        }

        start = end;
        if (*start == ' ') {
            ++start;
        }
    }
    *p = NULL;
    return decoded;
}
    




int main() {
    char* decoded_payload = decodePayload(obf_data);
    return 0;
}
    


/* NOTES

// Structure pour simuler un dictionnaire de type map
typedef struct {
    const char* word;
    BYTE value;
} WordDict;

// Définition du dictionnaire
WordDict wordDict[] = {
    {"naval", 0x00},
    {"trainers", 0x01},
    {"particle", 0x02},
    {"graham", 0x03},
    {"minor", 0x04}
};

#define DICT_SIZE (sizeof(wordDict) / sizeof(wordDict[0]))

*/