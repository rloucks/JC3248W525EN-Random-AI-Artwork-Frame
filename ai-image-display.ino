#define CHINSCREEN_ENABLE_MEDIA
#define CHINSCREEN_ENABLE_SHAPES

// Enable image format decoders
#define LV_USE_PNG 1

#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

// WiFi credentials
const char* ssid = "Slowww";
const char* password = "sunny loaf of bread";

// AI Stack endpoints
const char* SERVER_IP = "192.168.50.78";
const char* COMFYUI_PORT = "8188";
const char* OLLAMA_PORT = "11434";

// Image settings
const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 480;
const int SAMPLER_STEPS = 20;
const char* CHECKPOINT = "sdxl_complete.safetensors";
const char* NEGATIVE_PROMPT = "blurry, low quality, watermark, text";

// Timing settings
const unsigned long AUTO_REFRESH_INTERVAL_MS = 60000; // 2 minutes (changed from 5)

// Extensive prompt randomization arrays (500 entries each)
const char* SUBJECTS[] = {
"glass bottle","ancient tome","crystal sphere","ornate lantern","wooden chest","potion vial","mystical sword","floating orb","golden chalice","silver mirror",
"enchanted ring","glowing amulet","magical staff","ancient scroll","stone tablet","brass compass","ivory horn","jade statue","obsidian dagger","runic stone",
"hourglass","music box","telescope","dragon egg","phoenix feather","enchanted crown","jeweled dagger","mechanical bird","brass key","ancient mask",
"mythic harp","crystal dagger","alchemy set","rusted helm","engraved shield","sacred torch","glass slipper","haunted doll","sunstone","moonstone",
"meteor fragment","wolf pelt","unicorn horn","black pearl","giant conch","silver flute","bronze mask","jade figurine","cursed idol","ancient coins",
"royal scepter","feather quill","totem pole","dragon skull","druid staff","emerald brooch","map scroll","spyglass","magic carpet","crown of thorns",
"crystal rose","starlit orb","mirror shard","bone flute","sacred gem","scroll of fire","wand of ice","sword of lightning","armor of shadows","book of spells",
"sacred bell","ring of power","orb of wisdom","staff of destiny","mirror of truth","shield of ages","demon mask","angel feather","dragon scale","unicorn mane",
"phoenix ash","serpent idol","witchs cauldron","vampire fang","werewolf claw","elf crown","goblin totem","dwarf hammer","giants tooth","cyclops eye",
"kraken tentacle","mermaid comb","faerie wings","pegasus feather","griffin claw","centaur bow","sphinx relic","oracle bones","prophecy scroll","sacred lotus",
"crystal crown","burning torch","cursed gem","blessed relic","stone crown","silver harp","runestone","iron gauntlet","warhorn","helm of valor",
"mask of shadows","orb of eternity","chalice of life","blade of destiny","cloak of invisibility","boots of swiftness","ring of eternity","jewel of night","staff of creation","amulet of dawn",
"crown of dusk","mirror of eternity","dagger of betrayal","lamp of wishes","bottle of whispers","lantern of souls","pendant of time","harp of dreams","drum of storms","totem of spirits",
"idol of chaos","relic of order","stone of fate","pearl of wisdom","goblet of blood","flask of shadows","key of realms","scroll of eternity","sword of kings","dagger of thieves",
"mask of gods","helm of nightmares","orb of shadows","crown of light","gem of destiny","idol of eternity","staff of storms","dagger of shadows","amulet of whispers","orb of chaos",
"shield of fire","mirror of souls","blade of eternity","ring of dawn","lantern of dusk","scroll of shadows","crown of eternity","staff of shadows","chalice of eternity","flute of eternity",
"gem of shadows","torch of eternity","idol of shadows","cloak of eternity","armor of dawn","amulet of dusk","orb of light","scepter of shadows","book of eternity","dagger of light",
"mask of eternity","sword of dawn","key of shadows","harp of eternity","ring of shadows","jewel of eternity","crystal crown","phoenix stone","dragon claw","angelic harp",
"celestial sphere","ethereal bell","arcane crystal","haunted crown","burnt relic","storm gem","ocean pearl","sun relic","moon relic","star fragment",
"cosmic shard","fallen star","nebula crystal","galaxy stone","eternal flame","sacred relic","ancient blade","bloodstone","storm orb","ice shard",
"lava gem","forest gem","sky crystal","earth talisman","wind charm","fire amulet","shadow crown","light gem","dark relic","holy relic",
"serpent fang","beast claw","elemental stone","magic dice","seer s orb","time crystal","mystic bell","soul stone","dreamcatcher","ancient drum",
"battle horn","spirit mask","totem mask","jade crown","obsidian shard","arcane tome","gold idol","silver key","iron sword","bronze chalice",
"ruby crown","sapphire gem","emerald ring","diamond shard","onyx stone","amber crystal","opal jewel","topaz stone","pearl charm","ivory mask",
"coral crown","gilded harp","starlight stone","sunlit crown","moonlit gem","storm crown","tempest gem","rain shard","cloud crystal","mist stone",
"flame idol","inferno crown","ice crown","frost gem","blizzard stone","volcano relic","earth shard","mountain stone","river pearl","forest idol",
"jungle crown","desert gem","oasis stone","canyon relic","cave idol","sky shard","void relic","chaos crystal","order talisman","balance charm",
"creation tablet","destruction rune","memory locket","echo shell","hollow bell","guardian spear","sentinel badge","mystic lens","astral compass","hidden key",
"lost portrait","crystal vial","petrified rose","ghostly cloak","sapphire locket","golem core","spore pod","lifeblood vial","eldritch bone","ancient token",
"votive statue","rune blade","seafarer s sextant","cartographer s map","enchanted coin","magi s sigil","lost diary","herbal sachet","spellstone","silver thread",
"mirror locket","stargazer s monocle","warrior s medallion","harvest charm","winter timeglass","summer talisman","spring seed","autumn leaf","soul lantern","spirit bead",
"ether shard","plague vial","cure phial","mending cloth","void lantern","sun shard 201","moon shard 202","star shard 203","comet stone 204","nebula gem 205",
"ancient tome 206","crystal sphere 207","ornate lantern 208","wooden chest 209","potion vial 210","mystical sword 211","floating orb 212","golden chalice 213","silver mirror 214","enchanted ring 215",
"glowing amulet 216","magical staff 217","ancient scroll 218","stone tablet 219","brass compass 220","ivory horn 221","jade statue 222","obsidian dagger 223","runic stone 224","hourglass 225",
"music box 226","telescope 227","dragon egg 228","phoenix feather 229","enchanted crown 230","jeweled dagger 231","mechanical bird 232","brass key 233","ancient mask 234","mythic harp 235",
"crystal dagger 236","alchemy set 237","rusted helm 238","engraved shield 239","sacred torch 240","glass slipper 241","haunted doll 242","sunstone 243","moonstone 244","meteor fragment 245",
"wolf pelt 246","unicorn horn 247","black pearl 248","giant conch 249","silver flute 250","bronze mask 251","jade figurine 252","cursed idol 253","ancient coins 254","royal scepter 255",
"glass orb 256","feather quill 257","enchanted harp 258","stone idol 259","totem pole 260","dragon skull 261","druid staff 262","emerald brooch 263","map scroll 264","spyglass 265",
"magic carpet 266","crown of thorns 267","crystal rose 268","starlit orb 269","mirror shard 270","bone flute 271","sacred gem 272","scroll of fire 273","wand of ice 274","sword of lightning 275",
"armor of shadows 276","book of spells 277","sacred bell 278","ring of power 279","orb of wisdom 280","staff of destiny 281","mirror of truth 282","shield of ages 283","demon mask 284","angel feather 285",
"dragon scale 286","unicorn mane 287","phoenix ash 288","serpent idol 289","witchs cauldron 290","vampire fang 291","werewolf claw 292","elf crown 293","goblin totem 294","dwarf hammer 295",
"giants tooth 296","cyclops eye 297","kraken tentacle 298","mermaid comb 299","faerie wings 300","pegasus feather 301","griffin claw 302","centaur bow 303","sphinx relic 304","oracle bones 305",
"prophecy scroll 306","sacred lotus 307","crystal crown 308","burning torch 309","cursed gem 310","blessed relic 311","stone crown 312","silver harp 313","runestone 314","iron gauntlet 315",
"warhorn 316","helm of valor 317","mask of shadows 318","orb of eternity 319","chalice of life 320","blade of destiny 321","cloak of invisibility 322","boots of swiftness 323","ring of eternity 324","jewel of night 325",
"staff of creation 326","amulet of dawn 327","crown of dusk 328","mirror of eternity 329","dagger of betrayal 330","lamp of wishes 331","bottle of whispers 332","lantern of souls 333","pendant of time 334","harp of dreams 335",
"drum of storms 336","totem of spirits 337","idol of chaos 338","relic of order 339","stone of fate 340","pearl of wisdom 341","goblet of blood 342","flask of shadows 343","key of realms 344","scroll of eternity 345",
"sword of kings 346","dagger of thieves 347","mask of gods 348","helm of nightmares 349","orb of shadows 350","crown of light 351","gem of destiny 352","idol of eternity 353","staff of storms 354","dagger of shadows 355",
"amulet of whispers 356","orb of chaos 357","shield of fire 358","mirror of souls 359","blade of eternity 360","ring of dawn 361","lantern of dusk 362","scroll of shadows 363","crown of eternity 364","staff of shadows 365",
"chalice of eternity 366","flute of eternity 367","gem of shadows 368","torch of eternity 369","idol of shadows 370","cloak of eternity 371","armor of dawn 372","amulet of dusk 373","orb of light 374","scepter of shadows 375",
"book of eternity 376","dagger of light 377","mask of eternity 378","sword of dawn 379","key of shadows 380","harp of eternity 381","ring of shadows 382","jewel of eternity 383","crystal crown 384","phoenix stone 385",
"dragon claw 386","angelic harp 387","celestial sphere 388","ethereal bell 389","arcane crystal 390","haunted crown 391","burnt relic 392","storm gem 393","ocean pearl 394","sun relic 395",
"moon relic 396","star fragment 397","cosmic shard 398","fallen star 399","nebula crystal 400","galaxy stone 401","eternal flame 402","sacred relic 403","ancient blade 404","bloodstone 405",
"storm orb 406","ice shard 407","lava gem 408","forest gem 409","sky crystal 410","earth talisman 411","wind charm 412","fire amulet 413","shadow crown 414","light gem 415",
"dark relic 416","holy relic 417","serpent fang 418","beast claw 419","elemental stone 420","magic dice 421","seer s orb 422","time crystal 423","mystic bell 424","soul stone 425",
"dreamcatcher 426","ancient drum 427","battle horn 428","spirit mask 429","totem mask 430","jade crown 431","obsidian shard 432","arcane tome 433","gold idol 434","silver key 435",
"iron sword 436","bronze chalice 437","ruby crown 438","sapphire gem 439","emerald ring 440","diamond shard 441","onyx stone 442","amber crystal 443","opal jewel 444","topaz stone 445",
"pearl charm 446","ivory mask 447","coral crown 448","gilded harp 449","starlight stone 450","sunlit crown 451","moonlit gem 452","storm crown 453","tempest gem 454","rain shard 455",
"cloud crystal 456","mist stone 457","flame idol 458","inferno crown 459","ice crown 460","frost gem 461","blizzard stone 462","volcano relic 463","earth shard 464","mountain stone 465",
"river pearl 466","forest idol 467","jungle crown 468","desert gem 469","oasis stone 470","canyon relic 471","cave idol 472","sky shard 473","void relic 474","chaos crystal 475",
"order talisman 476","balance charm 477","creation tablet 478","destruction rune 479","memory locket 480","echo shell 481","hollow bell 482","guardian spear 483","sentinel badge 484","mystic lens 485",
"astral compass 486","hidden key 487","lost portrait 488","crystal vial 489","petrified rose 490","ghostly cloak 491","sapphire locket 492","golem core 493","spore pod 494","lifeblood vial 495",
"eldritch bone 496","ancient token 497","votive statue 498","rune blade 499","seafarer s sextant 500"
};

const char* LOCATIONS[] = {
"mystical forest","mountain peak","desert oasis","ocean cliff","ancient temple","starry night","misty valley","frozen tundra","volcanic crater","crystal cave",
"bamboo grove","floating island","underground cavern","moonlit garden","stormy sea","cherry blossom grove","autumn woodland","snowy mountains","tropical jungle","rocky canyon",
"enchanted meadow","haunted castle","hidden waterfall","abandoned village","forgotten ruins","sacred shrine","giant s lair","goblin camp","fairy glade","dwarf mine",
"ancient coliseum","undead crypt","wizard tower","sorcerer s den","burning battlefield","eternal desert","glacial palace","starlit observatory","celestial plain","thunder plateau",
"whispering woods","shadow swamp","mystic river","radiant cliffs","forgotten catacombs","sunken temple","cursed battlefield","heavenly garden","abyssal trench","eternal night",
"lava fields","glowing cave","serpent canyon","oracle s temple","enchanted bridge","ruined monastery","sapphire lake","obsidian ridge","crystal plateau","emerald plains",
"shimmering dunes","crimson wasteland","silent marsh","twilight grove","golden savanna","endless horizon","eternal blizzard","rainbow valley","hollow canyon","haunted woods",
"dragon s nest","phoenix roost","unicorn meadow","griffin cliffs","centaur plains","mermaid lagoon","kraken s trench","cyclops hill","golem valley","shadow fortress",
"crystal desert","obsidian plains","lava canyon","thunder storm","misty archipelago","glacial cliffside","sacred mountain","floating palace","hidden grotto","mystical swamp",
"ancient library","forgotten dungeon","underground city","celestial tower","fiery pit","icy cavern","starlit desert","cosmic void","dreamscape","phantom valley",
"eternal forest","whispering desert","howling canyon","burning plains","stormy ridge","golden cliffs","sapphire coast","emerald jungle","amethyst grove","onyx cavern",
"coral reef","abyssal cave","seaweed jungle","pirate bay","sunken ruins","ghost ship","frozen sea","lava lake","volcano peak","tundra expanse",
"desert plateau","arctic island","tropical beach","savanna plains","mangrove swamp","marble temple","iron fortress","stone citadel","sky fortress","cloud temple",
"rainbow bridge","misty bridge","ancient causeway","hidden staircase","secret passage","sacred altar","holy grove","forbidden lands","eternal canyon","infinite dunes",
"storm wasteland","blessed meadow","shimmering peaks","sacred grove","silent valley","golden fields","haunted crypt","fallen ruins","celestial palace","astral plains",
"obsidian cliffs","glowing canyon","emerald peaks","ruby valley","sapphire ridge","diamond desert","pearl coast","opal forest","amber jungle","topaz plains",
"twilight coast","midnight grove","dawn fields","sunset desert","moonlit lake","starlight valley","cosmic ridge","nebula plains","galactic desert","void expanse",
"mystic lagoon","enchanted quay","silversand beach","whispering marsh","stone circle","old watchtower","forgotten harbor","salt flats","mirror lake","silent isles",
"thorned valley","glass fen","ancient vineyard","ruined amphitheater","hollow hill","blood plain","witchs crossroads","ancient caravanserai","serpent marsh","frozen waterfall",
"sunken city","meteor crater","timberland","pine barrens","willow glade","crystal fjord","springs of youth","lotus pond","ivy covered ruins","wind swept plateau",
"frozen reef","ember fields","ash desert","cinder dunes","sapphire inlet","gilded bay","copper canyon","mossy dale","stalagmite hollow","echoing caverns",
"lost archipelago","stargazer s overlook","monarch s meadow","royal gardens","victors field","harvest prairie","hunter s hollow","bandit hideout","traders crossing","ancient gate",
"silver mines","iron hills","forgotten quarry","mystic falls","bloodstone basin","celestial bridge","sea of glass","mirror sands","crystal lagoon","emerald grotto",
"ivory cliffs","gargoyle spires","mariners reef","boreal reach","frozen steppe","glacier bay","ember coast","sunfire ridge","moonshadow hollow","opal shore",
"thunderhead plain","hailstone ridge","seraphim gardens","fallen spire","twilight terrace","gloomfen","pearl marsh","amber flats","sundial plateau","old battlefield",
"ancient observatory","wanderer s crossroads","trapper s camp","ironclad harbor","fogbound inlet","saltmarsh hollow","sylvan thicket","drybone mesa","redstone vale","lonesome heath",
"mystic isle 201","mountain peak 202","desert oasis 203","ocean cliff 204","ancient temple 205","starry night 206","misty valley 207","frozen tundra 208","volcanic crater 209","crystal cave 210",
"bamboo grove 211","floating island 212","underground cavern 213","moonlit garden 214","stormy sea 215","cherry blossom grove 216","autumn woodland 217","snowy mountains 218","tropical jungle 219","rocky canyon 220",
"enchanted meadow 221","haunted castle 222","hidden waterfall 223","abandoned village 224","forgotten ruins 225","sacred shrine 226","giant s lair 227","goblin camp 228","fairy glade 229","dwarf mine 230",
"ancient coliseum 231","undead crypt 232","wizard tower 233","sorcerer s den 234","burning battlefield 235","eternal desert 236","glacial palace 237","starlit observatory 238","celestial plain 239","thunder plateau 240",
"whispering woods 241","shadow swamp 242","mystic river 243","radiant cliffs 244","forgotten catacombs 245","sunken temple 246","cursed battlefield 247","heavenly garden 248","abyssal trench 249","eternal night 250",
"lava fields 251","glowing cave 252","serpent canyon 253","oracle s temple 254","enchanted bridge 255","ruined monastery 256","sapphire lake 257","obsidian ridge 258","crystal plateau 259","emerald plains 260",
"shimmering dunes 261","crimson wasteland 262","silent marsh 263","twilight grove 264","golden savanna 265","endless horizon 266","eternal blizzard 267","rainbow valley 268","hollow canyon 269","haunted woods 270",
"dragon s nest 271","phoenix roost 272","unicorn meadow 273","griffin cliffs 274","centaur plains 275","mermaid lagoon 276","kraken s trench 277","cyclops hill 278","golem valley 279","shadow fortress 280",
"crystal desert 281","obsidian plains 282","lava canyon 283","thunder storm 284","misty archipelago 285","glacial cliffside 286","sacred mountain 287","floating palace 288","hidden grotto 289","mystical swamp 290",
"ancient library 291","forgotten dungeon 292","underground city 293","celestial tower 294","fiery pit 295","icy cavern 296","starlit desert 297","cosmic void 298","dreamscape 299","phantom valley 300",
"eternal forest 301","whispering desert 302","howling canyon 303","burning plains 304","stormy ridge 305","golden cliffs 306","sapphire coast 307","emerald jungle 308","amethyst grove 309","onyx cavern 310",
"coral reef 311","abyssal cave 312","seaweed jungle 313","pirate bay 314","sunken ruins 315","ghost ship 316","frozen sea 317","lava lake 318","volcano peak 319","tundra expanse 320",
"desert plateau 321","arctic island 322","tropical beach 323","savanna plains 324","mangrove swamp 325","marble temple 326","iron fortress 327","stone citadel 328","sky fortress 329","cloud temple 330",
"rainbow bridge 331","misty bridge 332","ancient causeway 333","hidden staircase 334","secret passage 335","sacred altar 336","holy grove 337","forbidden lands 338","eternal canyon 339","infinite dunes 340",
"storm wasteland 341","blessed meadow 342","shimmering peaks 343","sacred grove 344","silent valley 345","golden fields 346","haunted crypt 347","fallen ruins 348","celestial palace 349","astral plains 350",
"obsidian cliffs 351","glowing canyon 352","emerald peaks 353","ruby valley 354","sapphire ridge 355","diamond desert 356","pearl coast 357","opal forest 358","amber jungle 359","topaz plains 360",
"twilight coast 361","midnight grove 362","dawn fields 363","sunset desert 364","moonlit lake 365","starlight valley 366","cosmic ridge 367","nebula plains 368","galactic desert 369","void expanse 370",
"mystic lagoon 371","enchanted quay 372","silversand beach 373","whispering marsh 374","stone circle 375","old watchtower 376","forgotten harbor 377","salt flats 378","mirror lake 379","silent isles 380",
"thorned valley 381","glass fen 382","ancient vineyard 383","ruined amphitheater 384","hollow hill 385","blood plain 386","witchs crossroads 387","ancient caravanserai 388","serpent marsh 389","frozen waterfall 390",
"sunken city 391","meteor crater 392","timberland 393","pine barrens 394","willow glade 395","crystal fjord 396","springs of youth 397","lotus pond 398","ivy covered ruins 399","wind swept plateau 400",
"frozen reef 401","ember fields 402","ash desert 403","cinder dunes 404","sapphire inlet 405","gilded bay 406","copper canyon 407","mossy dale 408","stalagmite hollow 409","echoing caverns 410",
"lost archipelago 411","stargazer s overlook 412","monarch s meadow 413","royal gardens 414","victors field 415","harvest prairie 416","hunter s hollow 417","bandit hideout 418","traders crossing 419","ancient gate 420",
"silver mines 421","iron hills 422","forgotten quarry 423","mystic falls 424","bloodstone basin 425","celestial bridge 426","sea of glass 427","mirror sands 428","crystal lagoon 429","emerald grotto 430",
"ivory cliffs 431","gargoyle spires 432","mariners reef 433","boreal reach 434","frozen steppe 435","glacier bay 436","ember coast 437","sunfire ridge 438","moonshadow hollow 439","opal shore 440",
"thunderhead plain 441","hailstone ridge 442","seraphim gardens 443","fallen spire 444","twilight terrace 445","gloomfen 446","pearl marsh 447","amber flats 448","sundial plateau 449","old battlefield 450",
"ancient observatory 451","wanderer s crossroads 452","trapper s camp 453","ironclad harbor 454","fogbound inlet 455","saltmarsh hollow 456","sylvan thicket 457","drybone mesa 458","redstone vale 459","lonesome heath 460",
"mystic isle 461","windward point 462","stormwatch keep 463","sunspire ridge 464","silverwood 465","golden fen 466","crystal glen 467","ember hollow 468","frostfen 469","shadowfen 470",
"veil of mists 471","mirror of skies 472","the hollow 473","the lowlands 474","the highreach 475","the blackfen 476","the brightfield 477","the deepwild 478","the ironmarch 479","the eastward trail 480",
"the westward plain 481","the northern wilds 482","the southern shoals 483","southern wastes 484","northern crags 485","old kings road 486","merchant s way 487","pilgrims crossing 488","hermits hollow 489","seers overlook 490",
"wanderers rest 491","mourners field 492","champions hold 493","the sunken way 494","the silver path 495","the thorn road 496","the star road 497","the moon road 498","the blood road 499","the lost trail 500"
};

const char* COLORS[] = {
"purple galaxy","golden sunset","emerald green","sapphire blue","crimson red","silver moonlight","amber glow","cosmic nebula","turquoise waters","rose gold",
"deep violet","fiery orange","icy white","jade green","midnight blue","coral pink","bronze shimmer","pearl white","obsidian black","rainbow spectrum",
"opal shimmer","garnet red","platinum shine","topaz yellow","cobalt blue","ruby sparkle","diamond shine","onyx black","ivory white","amethyst purple",
"aqua teal","peach blush","charcoal grey","forest green","sky blue","sunset orange","dawn pink","twilight purple","solar gold","lunar silver",
"ashen grey","blood red","ghost white","lava orange","frost blue","sand beige","moss green","cloud white","storm grey","honey yellow",
"mint green","cherry red","bubblegum pink","royal blue","steel grey","olive green","maroon red","champagne gold","gunmetal grey","pearl pink",
"arctic blue","sunburst yellow","volcano red","pine green","cobalt teal","rust orange","seafoam green","obsidian grey","celestial silver","cosmic purple",
"neon green","electric blue","laser pink","holographic silver","prism rainbow","crystal teal","burnt umber","mahogany brown","desert sand","tundra white",
"ashen blue","ivory beige","graphite grey","smoky black","frosted silver","aurora green","nebula pink","galaxy blue","supernova yellow","plasma purple",
"infrared red","ultraviolet violet","celestial gold","dusk purple","dawn orange","twilight blue","midday yellow","midnight black","storm purple","glimmer teal",
"moon white","sun gold","star silver","comet blue","meteor red","eclipse black","aurora pink","nebula violet","cosmic teal","galactic green",
"hologram silver","iridescent pearl","vanta black","chromatic gold","spectral white","quantum blue","atomic green","plasma teal","fusion red","radiant yellow",
"shimmer pink","fluorescent green","laser violet","starlight silver","moonstone grey","opal white","amber orange","emerald teal","ruby red","sapphire cyan",
"citrine glow","peridot green","selenite white","smoke blue","brick red","walnut brown","sandalwood","saffron gold","cerulean blue","afternoon haze",
"morning mist","evening ember","polar night","desert rose","glacier mint","stormy indigo","velvet maroon","petal pink","lichen green","pond blue",
"copper sheen","brass tone","steel blue","cloudburst","pearl grey","smokewood","dawn lilac","orchid haze","ginger spice","clay brown",
"pale wheat","sierra beige","cobalt dusk","midnight teal","ballet pink","antique silver","vintage gold","rustic copper","industrial grey","sea glass",
"ocean deep","moss moss","fern green","kelp green","coral reef","tropical teal","lilac mist","berry jam","plum dusk","maple syrup",
"ginger root","lemon zest","mustard seed","cinnamon bark","cocoa bean","espresso brown","charcoal smoke","lead grey","pearl blush","opal dusk",
"nebula teal","quantum violet","polaris white","aurora borealis green","meteorite grey","starlight cyan","sunflare orange","moonbeam silver","galaxy magenta","cosmos indigo",
"velvet rose","ceramic white","chalkboard black","paper antique","linen beige","twine brown","cobalt flame","azure twilight","mint frost","sage whisper",
"ivy league","olive branch","basil leaf","rosemary green","bay leaf","lichen moss","sea salt","ocean spray","beach sand","driftwood",
"dusty rose","mulberry","cranberry","raspberry","strawberry milk","peony","hibiscus","lotus blush","orchid pink","blossom",
"mossy oak","pine needle","cedar green","bamboo stalk","willow green","laurel leaf","sagebrush","eucalyptus","tea leaf","moss carpet",
"nightshade","deep space","void black","starlit navy","cosmic slate","plasma blue","ion teal","solar flint","ember ash","lava core",
"sunset coral","sundown mauve","sunrise peach","dawn rose","evening plum","midday azure","noon sky","twilight marigold","midnight ember","stormcloud",
"seaspray","coastal mist","harbor grey","lighthouse white","pier blue","bay turquoise","reef aqua","tide pool","riptide","undertow",
"sunbeam 201","moonbeam 202","starlight 203","comet 204","nebula 205","galaxy 206","quantum 207","plasma 208","ion 209","flux 210",
"photon 211","particle 212","wave 213","particle dust 214","cosmic dust 215","celestial mist 216","orbital blue 217","lunar silver 218","solar gold 219","eclipse 220",
"radiant red 221","brilliant blue 222","glowing green 223","shimmering teal 224","aching amber 225","wistful white 226","sorrowful grey 227","joyous yellow 228","ancient bronze 229","mythic maroon 230",
"cryptic cyan 231","mystic mauve 232","enchanted emerald 233","sorcerer sapphire 234","wizard white 235","prism pink 236","optic orange 237","luminous lime 238","fizzy fuchsia 239","vivid vermillion 240",
"muted moss 241","faded fern 242","faint lilac 243","soft sand 244","gentle sky 245","subtle teal 246","calm cobalt 247","barely beige 248","whisper white 249","paper sky 250",
"electric lime 251","neon coral 252","hyper blue 253","ultra violet 254","infra white 255","infra blue 256","magnetic green 257","ionic red 258","solar flare 259","polar glow 260",
"ice crystal 261","frozen mist 262","winter sky 263","fairy dust 264","midsummer haze 265","autumn ember 266","harvest gold 267","withered leaf 268","twine 269","sable 270",
"coal 271","ink 272","raven 273","onyx 274","pewter 275","nickel 276","mercury 277","argent 278","bismuth 279","titanium 280",
"rosewood 281","auburn 282","copper 283","brass 284","ginger 285","cinnamon 286","sienna 287","umber 288","burnt sienna 289","mahogany 290",
"walnut 291","chestnut 292","hazel 293","bronze age 294","antique brass 295","patina 296","verdigris 297","seagrass 298","kelp 299","mangrove 300",
"coral blush 301","salmon 302","peach 303","apricot 304","mandarin 305","tangerine 306","pumpkin 307","copper glow 308","amber light 309","honeycomb 310",
"butterscotch 311","marigold 312","canary 313","dandelion 314","lemon 315","buttercup 316","custard 317","saffron 318","paprika 319","cardamom 320",
"emerald isle 321","jade pool 322","mint sprig 323","seafoam 324","oasis 325","cactus 326","sage 327","pickle 328","chartreuse 329","neon lawn 330",
"lapis 331","azurite 332","teal tide 333","cerulean 334","baltic 335","atlantic 336","pacific 337","indigo night 338","navy deep 339","marine 340",
"royal purple 341","imperial 342","violet dusk 343","orchid 344","lilac haze 345","wisteria 346","periwinkle 347","plum 348","mulberry 349","elderberry 350",
"lucent 351","gloss 352","matte 353","silk 354","satin 355","velvet 356","oxide 357","patina 358","flare 359","glint 360",
"gleam 361","spark 362","ember 363","glow 364","shine 365","sheen 366","polish 367","lustre 368","glisten 369","flicker 370",
"halo 371","aura 372","bloom 373","haze 374","veil 375","wash 376","tint 377","tone 378","shade 379","hue 380",
"tincture 381","pigment 382","chroma 383","spectrum 384","prism 385","refraction 386","iridescence 387","opalescent 388","lustrous pearl 389","shaded silver 390",
"gilded 391","bronzed 392","plated 393","forged 394","worn 395","vintage 396","aged 397","weathered 398","polished 399","brushed 400",
"patinated 401","oxidized 402","raw 403","pristine 404","clear 405","opaque 406","translucent 407","transparent 408","smoky 409","murky 410",
"crystalline 411","glassy 412","silvery blue 413","rosy dawn 414","dusky rose 415","sapphire dusk 416","golden hour 417","pearly dusk 418","silvered moon 419","inked night 420",
"verdant 421","blooming 422","floral 423","botanical 424","herbal 425","garden 426","meadow 427","pastel 428","soft 429","muted 430",
"vivid 431","brash 432","bold 433","electric 434","neon 435","fluorescent 436","lucid 437","clearcut 438","graphite 439","slate 440",
"storm 441","tempest 442","calm 443","serene 444","wild 445","tame 446","lush 447","sparse 448","dense 449","airy 450",
"thin 451","thick 452","rich 453","light 454","deep 455","bright 456","dull 457","brilliant 458","subdued 459","intense 460",
"muted glow 461","neon flash 462","soft dusk 463","harsh noon 464","evening chill 465","morning warmth 466","midnight frost 467","summer haze 468","winter glow 469","autumn fire 470",
"spring bloom 471","monsoon 472","dry season 473","harvest 474","boreal 475","tropic 476","polar 477","temperate 478","continental 479","maritime 480",
"continental dawn 481","coastal twilight 482","island morning 483","valley dusk 484","ridge morning 485","plateau noon 486","glen evening 487","hollow midnight 488","fen dawn 489","heath dusk 490",
"mesa glow 491","canyon hue 492","badlands 493","kiln red 494","potters clay 495","smoke stain 496","ink wash 497","linen dye 498","ocher tone 499","quartz pale 500"
};

const char* ATMOSPHERES[] = {
"ethereal lighting","dramatic shadows","soft glow","harsh sunlight","twilight ambiance","mystical fog","sparkling particles","dark mood","bright and airy","moody atmosphere",
"heavenly rays","misty haze","warm illumination","cool shadows","radiant beams","diffused light","sharp contrast","gentle warmth","mysterious darkness","brilliant shine",
"dreamlike haze","glowing mist","stormy tension","sunlit brilliance","eerie silence","heavenly aura","subtle shimmer","vivid glow","star-drenched","moonlit calm",
"chaotic storm","serene calm","majestic aura","radiant shimmer","sacred warmth","icy chill","magnetic glow","fading light","gilded ambiance","crystalline clarity",
"luminous haze","dim twilight","golden warmth","silver glow","celestial brilliance","phantom mist","stormy brilliance","haunting stillness","eternal twilight","bright flare",
"subdued mood","intense radiance","delicate glow","otherworldly shimmer","cosmic aura","dreamy silence","whispering light","glittering fog","sun-kissed","moon-kissed",
"shadowed brilliance","hazy warmth","celestial aura","cosmic shimmer","galactic glow","supernatural light","haunting shadows","mystical haze","golden brilliance","silver aura",
"starry brilliance","nebula haze","astral glow","eerie brilliance","sacred shimmer","angelic aura","divine radiance","blessed light","cursed shadow","arcane aura",
"fiery ambiance","icy brilliance","frozen silence","burning radiance","ethereal brilliance","transcendent glow","spectral haze","radiant twilight","sublime glow","infinite light",
"vaporous mist","glinting dust","scented breeze","electric charge","charged atmosphere","suspended motes","rain-swept","fogbound","clear sky","turbulent clouds",
"low fog","high haze","sultry heat","brisk chill","damp air","dry wind","salty spray","perfumed garden","musty crypt","sweeping gale",
"still air","whistling wind","crackling firelight","glowing embers","cold breath","warm hearth","sudden storm","gentle drizzle","steady rain","heavy downpour",
"ice crystals","frozen breath","glittering frost","drifting snow","driven blizzard","silken mist","lucid haze","prismatic fog","silvered mist","gossamer veil",
"murky smog","inky fog","sulfur haze","volcanic ash","ember smoke","candlelit gloom","lamp-lit warmth","neon glare","backlit haze","rim lighting",
"broad daylight","soft morning","noonday blaze","long shadows","short shadows","moon halo","eclipse gloom","aurora ribbons","meteor streaks","starlit canopy",
"enchanted glow","spiritual sheen","haunted hush","ominous gloom","radiant pulse","pulsing light","strobe flash","cinematic haze","film grain","soft focus",
"hyperreal clarity","crisp detail","painted light","gilded glow","bronzed light","silvered sheen","ivory shine","pearly light","opaline mist","mirror reflection",
"refraction scatter","lens flare","glare bloom","specular highlight","ambient occlusion","softbox glow","rim silhouette","backlit silhouette","silhouette dusk","silhouette dawn",
"low key","high key","contrasty light","even flat light","snow lit","underwater light","caustic patterns","dappled light","filtered light","veiled glow",
"streaked light","beaming shafts","god rays","crepuscular rays","smoke shafts","suspended sparks","dust motes","leafy bokeh","city bokeh","sparse glow",
"dense gloom","velvet dusk","silken dark","iron dusk","lead sky","pale dawn","hushed twilight","nocturnal hush","sunset haze","sunrise mist",
"afternoon glare","evening ember","royal dusk","twilit hush","moonglow veil","starglow","night shimmer","first light","last light","neon haze",
"flickering torchlight","torchlit corridor","candle circle","lantern glow","lantern fog","firefly scatter","glowing spores","bioluminescent sheen","deep ocean gloom","abyssal glow",
"cave luminescence","glacial shine","ice prism light","salt flats glare","mirage haze","heat shimmer","hiker s dawn","traveler s dusk","market bustle","silent temple quiet",
"rainbow mist","prism light","opal haze","iris glow","sunglint","moonbeams on water","cloudbreak","stormbreak","horizon blaze","distant lightning",
"thunder rumble","electric haze","ionized air","magnetic shimmer","psionic haze","mystic wind","spirit breeze","phantom light","otherworld veil","aether glow",
"spectral shimmer","soullight","ghostly pall","wraith mist","cryptic haze","ancient radiance","primeval mist","eldritch glow","eldritch fog","arcane shimmer",
"sigil glow","glyphic sheen","runic light","mana swirl","mana mist","etheric stream","aura pool","psychic sheen","thought haze","dream fog",
"lucid dreamlight","somber lighting","sombre twilight","pensive dusk","melancholy haze","wistful glow","nostalgic hue","sepia tone","vintage film","old photo grain",
"cinema glow","theatrical spot","stage wash","studio key","fill light","catch light","beauty light","rim flare 201","rim flare 202","rim flare 203",
"spotlight cold 204","spotlight warm 205","ambient evening 206","ambient morning 207","festive lights 208","fair lights 209","party glow 210",
"industrial haze 211","factory smog 212","tunnel gloom 213","subway glare 214","streetlamp glow 215","alley fog 216","neon alley 217","rainy neon 218","city mist 219","metropolis glow 220",
"countryside dawn 221","farmstead dusk 222","barn light 223","harvest haze 224","orchard light 225","vineyard dusk 226","lighthouse beam 227","coastal fog 228","harbor mist 229","ferry light 230",
"moonlit tide 231","salt wind 232","seabreeze 233","cliff fog 234","beach glow 235","dune haze 236","mirrored lagoon 237","lagoon glow 238","atoll light 239","reef shimmer 240",
"jungle canopy light 241","understory gloom 242","canopy shafts 243","rainforest mist 244","mangrove dusk 245","swamp sheen 246","bog fog 247","fen light 248","marsh glimmer 249","reed haze 250",
"highland mist 251","moor gloom 252","peat smoke 253","heath light 254","tarn shimmer 255","loch mist 256","stone circle dawn 257","stone circle dusk 258","barrow light 259","graveyard pall 260",
"crypt light 261","relic glow 262","relic haze 263","shrine glow 264","altar light 265","sacred hush 266","chant echo 267","procession torch 268","mandala glow 269","monk s dawn 270",
"temple dusk 271","pagoda light 272","bamboo shade 273","garden mist 274","bonsai glow 275","zen hush 276","meditation light 277","monastic twilight 278","hermit glow 279","oracle mist 280",
"sage smoke 281","incense haze 282","ritual flame 283","consecrated light 284","blessed gleam 285","cursed gleam 286","malefic shadow 287","baleful glow 288","sanguine light 289","nocturne hush 290",
"opera glow 291","gallery light 292","studio haze 293","atelier light 294","canvas sheen 295","pigment haze 296","brushstroke light 297","palette wash 298","gilded gallery 299","museum hush 300",
"laboratory glare 301","alchemical gleam 302","magus lamp 303","scientist s fluorescents 304","incubation glow 305","specimen light 306","cathode hum 307","circuit glimmer 308","device pulse 309","portal flare 310",
"portal aurora 311","rift shimmer 312","tear glow 313","breach light 314","rift fog 315","dimensional sheen 316","interstice glow 317","intersection light 318","junction mist 319","nexus shine 320",
"core hum 321","reactor glow 322","engine flare 323","keel light 324","bridge console glow 325","cockpit haze 326","starboard gleam 327","port shadow 328","anchor gloom 329","helm glow 330",
"voyager dawn 331","navigator dusk 332","cartographer s light 333","explorer s night 334","mariner s mist 335","astral haze 336","stellar shine 337","cosmic dust 338","vacuum glint 339","hubble glare 340",
"orbital dusk 341","satellite shimmer 342","lunar dust 343","sunspot glow 344","magnetosphere sheen 345","auroral curtains 346","ion trail 347","comet tail 348","meteoroid streak 349","event horizon 350",
"blackhole glare 351","singularity hush 352","time dilation shimmer 353","chronal haze 354","temporal ripple 355","past echo 356","future gleam 357","memory light 358","relic shimmer 359","remnant glow 360",
"heritage hush 361","ancestors veil 362","lineage light 363","legacy gleam 364","heirloom sheen 365","patina of time 366","rusted memory 367","aged glow 368","antique haze 369","vintage gleam 370",
"neon retro 371","synthwave haze 372","vaporwave glow 373","outrun shine 374","cyber dusk 375","techno flash 376","glitch shimmer 377","pixel glow 378","vector sheen 379","retro film 380",
"analog warmth 381","digital cold 382","chip light 383","PCB gleam 384","solder shine 385","lead trace 386","wire hum 387","relay flicker 388","switch click light 389","indicator glow 390",
"led wash 391","oled glow 392","plasma flare 393","tube warmth 394","cathode hum 395","zenith light 396","nadir shadow 397","equatorial glow 398","polar chill 399","tropical heat 400",
"temperate breeze 401","continental shift 402","oceanic swell 403","seismic tremor 404","tectonic rumble 405","faultline dust 406","volcanic plume 407","hot spring steam 408","geyser burst 409","magma shimmer 410",
"iron sky 411","steel mist 412","concrete haze 413","asphalt glare 414","glass reflection 415","chrome gleam 416","mirror sheen 417","polished stone glow 418","cairn hush 419","monolith shadow 420",
"tower glow 421","spire light 422","keep ember 423","moat mist 424","battlement breeze 425","sentry light 426","banner flutter 427","forge blaze 428","anvil sparks 429","bell tower chime 430",
"market bustle 431","merchant stall light 432","lantern alley 433","cobblestone shimmer 434","wagon dust 435","stable warmth 436","smithy heat 437","bakery steam 438","brewery haze 439","tavern glow 440",
"inn hearth 441","lamp post puddle 442","streetlamp halo 443","footpath dusk 444","laneway mist 445","courtyard light 446","arcade sparkle 447","fountain sheen 448","plaza noon 449","square dusk 450",
"festival lights 451","parade glow 452","carnival shine 453","fireworks burst 454","lantern release 455","paper lantern drift 456","string lights 457","bonfire blaze 458","drum circle vibe 459","midnight revel 460",
"quiet dawn 461","misty festival 462","empty stage 463","abandoned fair 464","lost song 465","sung silence 466","reverberant arch 467","echo chamber 468","hushed gallery 469","singular focus 470",
"meditative hush 471","chanting light 472","scripted glow 473","manual lantern 474","oil lamp glow 475","wick flame 476","match strike 477","spark flare 478","ember drift 479","coal bed 480",
"candle cluster 481","altar candlelight 482","chapel hush 483","basilica light 484","cathedral shadow 485","minster glow 486","cloister twilight 487","choir hush 488","vesper light 489","matins mist 490",
"serenity 491","tempest 492","resonance 493","reverie 494","suspense 495","revelation 496","mystery 497","clarity 498","obscurity 499","apotheosis 500"
};

const char* STYLES[] = {
"highly detailed","photorealistic","fantasy art","cinematic","8k quality","oil painting style","watercolor","concept art","digital art","masterpiece",
"studio lighting","fine art","ultra realistic","artistic render","epic composition","professional photography","illustrated","dramatic lighting","vivid colors","stunning detail",
"matte painting","3d render","low poly","pixel art","retro style","gothic art","baroque","renaissance","impressionist","surrealist",
"minimalist","pop art","expressionist","cubist","art nouveau","line art","sketch","charcoal drawing","chalk pastel","ink illustration",
"mixed media","graffiti","street art","collage","manga style","comic book","anime style","sci-fi aesthetic","steampunk","cyberpunk",
"dieselpunk","biomechanical","dark fantasy","heroic fantasy","mythological","tribal art","indigenous art","prehistoric cave art","stone carving","calligraphy",
"futuristic render","retro-futurism","digital collage","glitch art","vaporwave","synthwave","outrun","pixel aesthetic","lofi art","concept sketch",
"isometric art","vector art","flat design","hyperrealism","symbolism","romanticism","post-impressionism","modernism","postmodernism","futurism",
"brutalism","constructivism","suprematism","dadaism","minimal digital","illustrative realism","photo montage","architectural rendering","fashion illustration","product mockup",
"environment concept","character design","creature design","mecha design","vehicle concept","prop design","weapon concept","costume concept","environment matte","game art",
"animated still","motion blur","long exposure","shallow depth of field","bokeh","tilt shift","vintage lens","anamorphic","wide angle","telephoto",
"macro","micro","cross processed","film grain","instant film","polaroid style","kodak vibe","fuji look","cinematic grade","color grade",
"high contrast","low contrast","soft focus","hard focus","etching","linocut","woodcut","stipple","pointillism","dotwork",
"halftone","screenprint","silkscreen","poster style","propaganda poster","minimal poster","infographic style","retro poster","vintage poster","neo-noir",
"noir","film noir","pulp illustration","pulp cover","paper cut","paper craft","origami style","vector illustration","flat illustration","linework",
"technical drawing","blueprint","schematic","diagrammatic","architectural plan","cutaway","exploded view","orthographic","isometric diagram","cartoon",
"children s book","storybook illustration","whimsical","cute chibi","kawaii","gyotaku style","fresco","mosaic","stained glass","tapestry",
"embroidery","needlework","tattoo flash","old map style","engraving","etching style","wood engraving","monotype","gouache","tempera",
"encaustic","fresco secco","sumi-e","nihonga","ukiyo-e","moku hanga","yangban","hanji","ink wash 201","sumi brush 202",
"paste paper 203","dip pen 204","speedpaint 205","painterly 206","brushwork 207","palette knife 208","impasto 209","glazing 210","sfumato 211","chiaroscuro 212",
"tenebrism 213","alla prima 214","grisaille 215","underpainting 216","wash 217","wet on wet 218","wet on dry 219","drybrush 220","scrumbling 221","glaze layer 222",
"line weight 223","contour line 224","cross hatching 225","stippling 226","scribble style 227","gesture drawing 228","figure study 229","anatomical study 230","fashion plate 231","editorial illustration 232",
"vector poster 233","glyphic 234","iconic 235","pictogram 236","emoji style 237","flat icon 238","material design 239","neumorphism 240","glassmorphism 241","skeuomorphism 242",
"retro computer 243","crt glow 244","scanline 245","dot matrix 246","vector graphics 247","early 3d 248","ray traced 249","path traced 250",
"photomanipulation 251","composite 252","double exposure 253","triptych 254","diptych 255","montage 256","posterize 257","solarize 258","negative 259","high key portrait 260",
"low key portrait 261","rim lit portrait 262","backlit portrait 263","environmental portrait 264","candid 265","studio portrait 266","street portrait 267","polished editorial 268","glamour 269","boudoir 270",
"advertising aesthetic 271","product catalog 272","ecommerce clean 273","lookbook 274","runway 275","high fashion 276","haute couture 277","avant garde 278","couture sketch 279","costume plate 280",
"fantastical 281","mythic realism 282","weirdcore 283","liminal space 284","aesthetic 285","anti-aesthetic 286","decayed beauty 287","ruin porn 288","sublime 289","grotesque 290",
"elegant 291","ornate 292","baroque flourish 293","rococo swirl 294","gothic silhouette 295","neo-gothic 296","vintage Victorian 297","Edwardian 298","regency 299","georgian 300",
"industrial 301","steampunk 302","brass and gears 303","rust and rivets 304","diesel age 305","art deco 306","art nouveau 307","mid century 308","atomic age 309","space age 310",
"brutalist 311","scandinavian minimal 312","japanese minimal 313","wabi sabi 314","organic modern 315","punk 316","grunge 317","underground zine 318","diary aesthetic 319","DIY 320",
"hand drawn 321","scribbled 322","marker art 323","felt pen 324","crayon 325","colored pencil 326","pastel 327","soft pastel 328","hard pastel 329","oil pastel 330",
"graphite 331","charcoal 332","conte 333","chalk 334","chalkboard 335","stencil 336","spray paint 337","marker pen 338","felt tip 339","fineliner 340",
"ink wash 341","pen and ink 342","dip ink 343","gouache 344","acrylic 345","acrylic pour 346","spray enamel 347","encaustic wax 348","latex paint 349","house paint 350",
"photoreal car 351","automotive render 352","product render 353","glass render 354","metal render 355","fabric render 356","fur render 357","hair render 358","skin shader 359","subsurface scatter 360",
"procedural texture 361","hand painted texture 362","tiling texture 363","seamless repeat 364","uv mapped 365","normal mapped 366","displacement 367","microdetails 368","wear and tear 369","aging 370",
"weathered 371","dented 372","scuffed 373","polished 374","brushed metal 375","oxidized finish 376","patina finish 377","lacquered 378","varnished 379","matte finish 380",
"glossy finish 381","satin finish 382","silky 383","sheer 384","textured 385","grainy 386","pitted 387","porous 388","smooth 389","velvet 390",
"linen 391","canvas 392","burlap 393","hessian 394","suede 395","leather 396","suede suede 397","denim 398","wool 399","cotton 400",
"silk 401","satin 402","velour 403","tweed 404","tartan 405","paisley 406","baroque pattern 407","damask 408","floral 409","geometric 410",
"tribal pattern 411","aztec 412","mayan 413","inca 414","celtic knot 415","norse runes 416","hieroglyphic 417","glyphic 418","ornamental 419","filigree 420",
"lacework 421","beaded 422","embroidery 423","applique 424","quilted 425","patched 426","stitched 427","woven 428","knitted 429","crocheted 430",
"handmade 431","artisan 432","craft 433","studio 434","atelier 435","workshop 436","forge 437","smith 438","luthier 439","carpenter 440",
"glassblower 441","ceramist 442","potter 443","weaver 444","dyer 445","tanner 446","cobbler 447","metalworker 448","stonecutter 449","stonemason 450",
"landscape 451","portrait 452","still life 453","abstract 454","conceptual 455","installation 456","performance 457","interactive 458","immersive 459","experiential 460",
"generative 461","procedural 462","ai assisted 463","neural style transfer 464","style mix 465","mashup 466","remix 467","reinterpretation 468","reimagined 469","reworked 470",
"collector s edition 471","archival 472","museum grade 473","gallery print 474","limited edition 475","open edition 476","signed print 477","artist proof 478","folio 479","portfolio 480",
"conceptual study 481","thumbnail sketch 482","final render 483","color study 484","value study 485","composition study 486","storyboard 487","animatic 488","frame study 489","keyframe 490",
"polished concept 491","pitch render 492","visual development 493","lookdev 494","shader study 495","lighting study 496","texture pass 497","model pass 498","rig test 499","final plate 500"
};



const int SUBJECTS_COUNT = sizeof(SUBJECTS) / sizeof(SUBJECTS[0]);
const int LOCATIONS_COUNT = sizeof(LOCATIONS) / sizeof(LOCATIONS[0]);
const int COLORS_COUNT = sizeof(COLORS) / sizeof(COLORS[0]);
const int ATMOSPHERES_COUNT = sizeof(ATMOSPHERES) / sizeof(ATMOSPHERES[0]);
const int STYLES_COUNT = sizeof(STYLES) / sizeof(STYLES[0]);

// Display state
enum DisplayState {
    STATE_LOADING,
    STATE_GENERATING_PROMPT,
    STATE_GENERATING_IMAGE,
    STATE_DISPLAYING_IMAGE,
    STATE_ERROR
};

DisplayState currentState = STATE_LOADING;
lv_obj_t* currentImage = nullptr;
String currentPrompt = "";
int imageCount = 0;
uint16_t* globalImgData = nullptr;  // Track allocated image data for cleanup

// Touch event callback
static void touch_event_handler(lv_event_t* e) {
    if (currentState == STATE_DISPLAYING_IMAGE) {
        Serial.println("Touch detected - triggering refresh!");
        void triggerNewImageGeneration(); // Forward declaration
        triggerNewImageGeneration();
    }
}

// Forward declarations
void changeState(DisplayState newState);
void showErrorState(const char* msg);
bool initializeSPIFFS();
void connectToWiFi();
void showGeneratingPromptUI();
void generatePromptViaOllama();
void showGeneratingImageUI();
void generateImageViaComfyUI();
void waitAndDownloadImage(const String& promptId);
bool downloadAndStreamImage(const String& historyJson);
void displayImageFromSPIFFS();
void triggerNewImageGeneration();

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n\n=== AI Image Display Starting ===");
    
    // Initialize display
    init_display();
    
    // Startup screen
    chinScreen_background_solid("black");
    chinScreen_text("AI Image Display", 10, 10, "white", "large");
    chinScreen_text("Initializing...", 10, 40, "white", "medium");
    
    // Initialize SPIFFS
    if (!initializeSPIFFS()) {
        showErrorState("SPIFFS failed");
        while(1) delay(1000);
    }
    
    chinScreen_text("SPIFFS OK", 10, 70, "green", "small");
    delay(500);
    
    // Connect to WiFi
    chinScreen_text("Connecting WiFi...", 10, 100, "white", "medium");
    connectToWiFi();
    
    if (WiFi.status() != WL_CONNECTED) {
        showErrorState("WiFi failed");
        return;
    }
    
    chinScreen_text("WiFi connected!", 10, 130, "green", "small");
    delay(1000);
    
    // Start generating
    triggerNewImageGeneration();
}

void loop() {
    chinScreen_task_handler();
    
    // Auto-refresh after 2 minutes
    static unsigned long displayStartTime = 0;
    
    if (currentState == STATE_DISPLAYING_IMAGE) {
        if (displayStartTime == 0) {
            displayStartTime = millis();
        } else if (millis() - displayStartTime > AUTO_REFRESH_INTERVAL_MS) {
            displayStartTime = 0;
            Serial.println("Auto-refresh triggered (2 minutes elapsed)");
            triggerNewImageGeneration();
        }
    } else {
        displayStartTime = 0;
    }
    
    delay(50);
}

void triggerNewImageGeneration() {
    // Clean up memory BEFORE starting new generation
    if (globalImgData != nullptr) {
        Serial.println("Freeing previous image data...");
        free(globalImgData);
        globalImgData = nullptr;
    }
    
    if (currentImage != nullptr) {
        lv_obj_del(currentImage);
        currentImage = nullptr;
    }
    
    delay(100);  // Brief pause for cleanup
    changeState(STATE_GENERATING_PROMPT);
}

void changeState(DisplayState newState) {
    currentState = newState;
    delay(100);
    
    switch (currentState) {
        case STATE_GENERATING_PROMPT:
            showGeneratingPromptUI();
            generatePromptViaOllama();
            break;
        case STATE_GENERATING_IMAGE:
            showGeneratingImageUI();
            generateImageViaComfyUI();
            break;
        case STATE_DISPLAYING_IMAGE:
            displayImageFromSPIFFS();
            break;
        case STATE_ERROR:
            break;
        default:
            break;
    }
}

void showErrorState(const char* msg) {
    currentState = STATE_ERROR;
    Serial.printf("ERROR: %s\n", msg);
    chinScreen_clear();
    chinScreen_background_solid("darkred");
    chinScreen_text("Error!", 10, 10, "white", "large");
    chinScreen_text(msg, 10, 50, "yellow", "medium");
}

bool initializeSPIFFS() {
    Serial.print("Initializing SPIFFS...");
    
    if (!SPIFFS.begin(true)) {
        Serial.println(" failed!");
        return false;
    }
    
    Serial.println(" success!");
    
    size_t totalBytes = SPIFFS.totalBytes();
    size_t usedBytes = SPIFFS.usedBytes();
    Serial.printf("SPIFFS - Total: %d, Used: %d, Free: %d\n", 
                  totalBytes, usedBytes, totalBytes - usedBytes);
    
    // Clean up old temp files
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
        String filename = file.name();
        file.close();
        if (filename.indexOf("temp") >= 0) {
            SPIFFS.remove(filename);
            Serial.println("Removed: " + filename);
        }
        file = root.openNextFile();
    }
    root.close();
    
    return true;
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println("\nWiFi connection failed!");
    }
}

void showGeneratingPromptUI() {
    chinScreen_clear();
    chinScreen_background_solid("navy");
    chinScreen_text("Creating prompt...", 10, 10, "white", "large");
    chinScreen_text(String("Image #" + String(imageCount + 1)).c_str(), 10, 40, "white", "medium");
}

void generatePromptViaOllama() {
    // Generate completely random prompt from arrays
    int subjectIdx = random(0, SUBJECTS_COUNT);
    int locationIdx = random(0, LOCATIONS_COUNT);
    int colorIdx = random(0, COLORS_COUNT);
    int atmosphereIdx = random(0, ATMOSPHERES_COUNT);
    int styleIdx = random(0, STYLES_COUNT);
    
    // Build varied prompt structure (randomize word order too)
    int structure = random(0, 3);
    
    if (structure == 0) {
        // Structure: [subject] in [location], [color], [atmosphere], [style]
        currentPrompt = String(SUBJECTS[subjectIdx]) + " in " + 
                       String(LOCATIONS[locationIdx]) + ", " +
                       String(COLORS[colorIdx]) + ", " +
                       String(ATMOSPHERES[atmosphereIdx]) + ", " +
                       String(STYLES[styleIdx]);
    } else if (structure == 1) {
        // Structure: [location] with [subject], [atmosphere], [color], [style]
        currentPrompt = String(LOCATIONS[locationIdx]) + " with " +
                       String(SUBJECTS[subjectIdx]) + ", " +
                       String(ATMOSPHERES[atmosphereIdx]) + ", " +
                       String(COLORS[colorIdx]) + ", " +
                       String(STYLES[styleIdx]);
    } else {
        // Structure: [color] [subject], [location], [atmosphere], [style]
        currentPrompt = String(COLORS[colorIdx]) + " " +
                       String(SUBJECTS[subjectIdx]) + ", " +
                       String(LOCATIONS[locationIdx]) + ", " +
                       String(ATMOSPHERES[atmosphereIdx]) + ", " +
                       String(STYLES[styleIdx]);
    }
    
    Serial.println("Generated random prompt: " + currentPrompt);
    delay(500);
    changeState(STATE_GENERATING_IMAGE);
}

void showGeneratingImageUI() {
    chinScreen_clear();
    chinScreen_background_solid("white");
    chinScreen_text("Generating image...", 10, 10, "black", "large");
    chinScreen_text("Please wait while it generates", 10, 40, "gray", "medium");
    chinScreen_text(currentPrompt.substring(0, 46).c_str(), 10, 90, "black", "small");
    chinScreen_text(currentPrompt.substring(46, 120).c_str(), 10, 100, "black", "small");
}

void generateImageViaComfyUI() {
    HTTPClient http;
    char url[100];
    snprintf(url, sizeof(url), "http://%s:%s/prompt", SERVER_IP, COMFYUI_PORT);
    
    Serial.println("\n=== Building ComfyUI Workflow ===");
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(90000);
    
    // Use DynamicJsonDocument for larger workflows
    DynamicJsonDocument doc(5120);
    doc["client_id"] = "esp32_client";
    
    JsonObject prompt = doc.createNestedObject("prompt");
    
    // Node 4: Checkpoint Loader
    JsonObject node4 = prompt.createNestedObject("4");
    node4["class_type"] = "CheckpointLoaderSimple";
    JsonObject inputs4 = node4.createNestedObject("inputs");
    inputs4["ckpt_name"] = CHECKPOINT;
    
    // Node 5: Empty Latent Image
    JsonObject node5 = prompt.createNestedObject("5");
    node5["class_type"] = "EmptyLatentImage";
    JsonObject inputs5 = node5.createNestedObject("inputs");
    inputs5["width"] = IMAGE_WIDTH;
    inputs5["height"] = IMAGE_HEIGHT;
    inputs5["batch_size"] = 1;
    
    // Node 6: Positive CLIP Text Encode
    JsonObject node6 = prompt.createNestedObject("6");
    node6["class_type"] = "CLIPTextEncode";
    JsonObject inputs6 = node6.createNestedObject("inputs");
    inputs6["text"] = currentPrompt;
    JsonArray clip_ref6 = inputs6.createNestedArray("clip");
    clip_ref6.add("4");
    clip_ref6.add(1);
    
    // Node 7: Negative CLIP Text Encode
    JsonObject node7 = prompt.createNestedObject("7");
    node7["class_type"] = "CLIPTextEncode";
    JsonObject inputs7 = node7.createNestedObject("inputs");
    inputs7["text"] = NEGATIVE_PROMPT;
    JsonArray clip_ref7 = inputs7.createNestedArray("clip");
    clip_ref7.add("4");
    clip_ref7.add(1);
    
    // Node 3: KSampler
    JsonObject node3 = prompt.createNestedObject("3");
    node3["class_type"] = "KSampler";
    JsonObject inputs3 = node3.createNestedObject("inputs");
    inputs3["seed"] = random(1, 1000000);
    inputs3["steps"] = SAMPLER_STEPS;
    inputs3["cfg"] = 8;
    inputs3["sampler_name"] = "euler";
    inputs3["scheduler"] = "normal";
    inputs3["denoise"] = 1;
    JsonArray model_ref = inputs3.createNestedArray("model");
    model_ref.add("4");
    model_ref.add(0);
    JsonArray pos_ref = inputs3.createNestedArray("positive");
    pos_ref.add("6");
    pos_ref.add(0);
    JsonArray neg_ref = inputs3.createNestedArray("negative");
    neg_ref.add("7");
    neg_ref.add(0);
    JsonArray lat_ref = inputs3.createNestedArray("latent_image");
    lat_ref.add("5");
    lat_ref.add(0);
    
    // Node 8: VAE Decode
    JsonObject node8 = prompt.createNestedObject("8");
    node8["class_type"] = "VAEDecode";
    JsonObject inputs8 = node8.createNestedObject("inputs");
    JsonArray samples_ref = inputs8.createNestedArray("samples");
    samples_ref.add("3");
    samples_ref.add(0);
    JsonArray vae_ref = inputs8.createNestedArray("vae");
    vae_ref.add("4");
    vae_ref.add(2);
    
    // Node 9: SaveImage (simple standard node)
    JsonObject node9 = prompt.createNestedObject("9");
    node9["class_type"] = "SaveImage";
    JsonObject inputs9 = node9.createNestedObject("inputs");
    inputs9["filename_prefix"] = "ComfyUI";
    JsonArray images_ref9 = inputs9.createNestedArray("images");
    images_ref9.add("8");
    images_ref9.add(0);
    
    String workflowString;
    serializeJson(doc, workflowString);
    
    Serial.println("=== WORKFLOW JSON (first 500 chars) ===");
    Serial.println(workflowString.substring(0, min(500, (int)workflowString.length())));
    Serial.println("=== END PREVIEW ===");
    Serial.printf("Total JSON size: %d bytes\n", workflowString.length());
    
    Serial.println("Sending workflow to ComfyUI...");
    int httpCode = http.POST(workflowString);
    
    Serial.printf("HTTP Response Code: %d\n", httpCode);
    
    if (httpCode == 200) {
        String response = http.getString();
        Serial.println("SUCCESS Response: " + response);
        
        StaticJsonDocument<512> responseDoc;
        DeserializationError error = deserializeJson(responseDoc, response);
        
        if (!error && responseDoc.containsKey("prompt_id")) {
            String promptId = responseDoc["prompt_id"];
            Serial.println("Prompt ID: " + promptId);
            http.end();
            waitAndDownloadImage(promptId);
            return;
        } else {
            Serial.println("ERROR: No prompt_id in response");
        }
    } else if (httpCode > 0) {
        String errorResponse = http.getString();
        Serial.println("=== ERROR RESPONSE ===");
        Serial.println(errorResponse);
        Serial.println("=== END ERROR ===");
    } else {
        Serial.printf("HTTP Error: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();
    showErrorState("ComfyUI workflow failed");
}

void waitAndDownloadImage(const String& promptId) {
    Serial.println("Waiting for ComfyUI to generate...");
    
    chinScreen_text("ComfyUI processing...", 10, 120, "orange", "medium");
    
    char historyUrl[150];
    snprintf(historyUrl, sizeof(historyUrl), "http://%s:%s/history/%s", 
             SERVER_IP, COMFYUI_PORT, promptId.c_str());
    
    int cnt = 10;
    for (int attempt = 0; attempt < 120; attempt++) {
      cnt = cnt + 15;
        chinScreen_text(String("-").c_str(), 
                    cnt, 150, "red", "medium");
        
        HTTPClient http;
        http.begin(historyUrl);
        
        int httpCode = http.GET();
        if (httpCode == 200) {
            String historyJson = http.getString();
            
            if (attempt % 10 == 0) {
                Serial.printf("Attempt %d - checking history...\n", attempt);
            }
            
            if (historyJson.indexOf("\"images\"") > 0 && 
                (historyJson.indexOf(".gif") > 0 || historyJson.indexOf(".png") > 0 || historyJson.indexOf(".jpg") > 0)) {
                Serial.println("Image ready!");
                http.end();
                
                if (downloadAndStreamImage(historyJson)) {
                    changeState(STATE_DISPLAYING_IMAGE);
                    return;
                }
                showErrorState("Download failed");
                return;
            }
        }
        
        http.end();
        delay(1000);
        yield();
    }
    
    showErrorState("Timeout waiting for image");
}

bool downloadAndStreamImage(const String& historyJson) {
    // Image generation is complete, now download RGB565 data from converter server
    Serial.println("Image ready, downloading RGB565 data from server...");
    
    char rgb565Url[100];
    snprintf(rgb565Url, sizeof(rgb565Url), "http://%s:8090/image.rgb565", SERVER_IP);
    
    Serial.printf("Downloading from: %s\n", rgb565Url);
    
    HTTPClient http;
    http.begin(rgb565Url);
    http.setTimeout(30000);
    
    int httpCode = http.GET();
    Serial.printf("Download HTTP code: %d\n", httpCode);
    
    if (httpCode != 200) {
        Serial.printf("RGB565 download failed: %d\n", httpCode);
        http.end();
        return false;
    }
    
    int contentLength = http.getSize();
    int expectedSize = IMAGE_WIDTH * IMAGE_HEIGHT * 2; // 2 bytes per pixel
    
    Serial.printf("Content length: %d bytes (expected: %d)\n", contentLength, expectedSize);
    
    if (contentLength != expectedSize) {
        Serial.println("WARNING: Size mismatch!");
    }
    
    const char* tempPath = "/temp_image.rgb565";
    if (SPIFFS.exists(tempPath)) {
        SPIFFS.remove(tempPath);
    }
    
    File file = SPIFFS.open(tempPath, FILE_WRITE);
    if (!file) {
        Serial.println("ERROR: SPIFFS open failed");
        http.end();
        return false;
    }
    
    WiFiClient* stream = http.getStreamPtr();
    uint8_t buffer[512];
    int totalBytes = 0;
    
    Serial.print("Downloading RGB565: ");
    while (http.connected() && totalBytes < contentLength) {
        int available = stream->available();
        if (available > 0) {
            int toRead = min(available, (int)sizeof(buffer));
            int bytesRead = stream->readBytes(buffer, toRead);
            file.write(buffer, bytesRead);
            totalBytes += bytesRead;
            
            if (totalBytes % 10240 == 0) {
                Serial.print(".");
            }
        }
        yield();
    }
    
    file.close();
    http.end();
    
    Serial.printf("\nDownloaded %d bytes\n", totalBytes);
    
    if (totalBytes < 1000) {
        Serial.println("ERROR: File too small");
        SPIFFS.remove(tempPath);
        return false;
    }
    
    return true;
}

void displayImageFromSPIFFS() {
    Serial.println("\n=== Displaying Image ===");
    
    chinScreen_clear();
    chinScreen_background_solid("black");
    
    const char* rgb565Path = "/temp_image.rgb565";
    
    if (!SPIFFS.exists(rgb565Path)) {
        Serial.println("ERROR: RGB565 file not found");
        showErrorState("Image file missing");
        return;
    }
    
    File imgFile = SPIFFS.open(rgb565Path, FILE_READ);
    if (!imgFile) {
        showErrorState("Cannot open file");
        return;
    }
    
    size_t fileSize = imgFile.size();
    int expectedSize = IMAGE_WIDTH * IMAGE_HEIGHT * 2;
    
    Serial.printf("RGB565 file size: %d bytes (expected: %d)\n", fileSize, expectedSize);
    
    if (fileSize < expectedSize) {
        imgFile.close();
        showErrorState("File incomplete");
        return;
    }
    
    // Free old data before allocating new
    if (globalImgData != nullptr) {
        Serial.println("Freeing old image data...");
        free(globalImgData);
        globalImgData = nullptr;
    }
    
    // Allocate buffer for RGB565 data
    globalImgData = (uint16_t*)malloc(IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(uint16_t));
    if (!globalImgData) {
        imgFile.close();
        showErrorState("Memory allocation failed");
        return;
    }
    
    // Read RGB565 data
    imgFile.read((uint8_t*)globalImgData, IMAGE_WIDTH * IMAGE_HEIGHT * 2);
    imgFile.close();
    
    Serial.println("RGB565 data loaded into memory");
    
    bsp_display_lock(0);
    
    // Clear previous image
    if (currentImage != nullptr) {
        lv_obj_del(currentImage);
        currentImage = nullptr;
    }
    
    // Create canvas to display raw RGB565 data
    currentImage = lv_canvas_create(lv_scr_act());
    if (!currentImage) {
        bsp_display_unlock();
        free(globalImgData);
        globalImgData = nullptr;
        showErrorState("Canvas create failed");
        return;
    }
    
    // Set canvas buffer (RGB565 format, no alpha)
    lv_canvas_set_buffer(currentImage, globalImgData, IMAGE_WIDTH, IMAGE_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    
    // Center the canvas
    lv_obj_center(currentImage);
    
    // Add touch event handler to the canvas for touch-to-refresh
    //lv_obj_add_flag(currentImage, LV_OBJ_FLAG_CLICKABLE);
    //lv_obj_add_event_cb(currentImage, touch_event_handler, LV_EVENT_CLICKED, NULL);
    
    bsp_display_unlock();
    
    Serial.println("=== SUCCESS - RGB565 Image displayed! ===");
    Serial.println("Touch screen to refresh or wait 2 minutes for auto-refresh\n");
    imageCount++;
}