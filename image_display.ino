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
const char* ssid = " "; //change this
const char* password = " "; //change this

// AI Stack endpoints
const char* SERVER_IP = "192.168.1.1"; //change this
const char* COMFYUI_PORT = "8188"; // your comfyui port
const char* OLLAMA_PORT = "11434"; //your ollama port

// Image settings
const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 480;
const int SAMPLER_STEPS = 20;
const char* CHECKPOINT = "sdxl_complete.safetensors";
const char* NEGATIVE_PROMPT = "blurry, low quality, watermark, text";

// Timing settings
const unsigned long AUTO_REFRESH_INTERVAL_MS = 60000; // 2 minutes (changed from 5)

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
"ether shard","plague vial","cure phial","mending cloth","void lantern","sun shard","moon shard","star shard","comet stone","nebula gem",
"glass orb"
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
"mystic isle","windward point","stormwatch keep","sunspire ridge","silverwood","golden fen","crystal glen","ember hollow","frostfen","shadowfen",
"veil of mists","mirror of skies","the hollow","the lowlands","the highreach","the blackfen","the brightfield","the deepwild","the ironmarch","the eastward trail",
"the westward plain","the northern wilds","the southern shoals","southern wastes","northern crags","old kings road","merchant s way","pilgrims crossing","hermits hollow","seers overlook",
"wanderers rest","mourners field","champions hold","the sunken way","the silver path","the thorn road","the star road","the moon road","the blood road","the lost trail"
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
"sunbeam","moonbeam","starlight","comet","nebula","galaxy","quantum","plasma","ion","flux",
"photon","particle","wave","particle dust","cosmic dust","celestial mist","orbital blue","lunar silver","solar gold","eclipse",
"radiant red","brilliant blue","glowing green","shimmering teal","aching amber","wistful white","sorrowful grey","joyous yellow","ancient bronze","mythic maroon",
"cryptic cyan","mystic mauve","enchanted emerald","sorcerer sapphire","wizard white","prism pink","optic orange","luminous lime","fizzy fuchsia","vivid vermillion",
"muted moss","faded fern","faint lilac","soft sand","gentle sky","subtle teal","calm cobalt","barely beige","whisper white","paper sky",
"electric lime","neon coral","hyper blue","ultra violet","infra white","infra blue","magnetic green","ionic red","solar flare","polar glow",
"ice crystal","frozen mist","winter sky","fairy dust","midsummer haze","autumn ember","harvest gold","withered leaf","twine","sable",
"coal","ink","raven","onyx","pewter","nickel","mercury","argent","bismuth","titanium",
"rosewood","auburn","copper","brass","ginger","cinnamon","sienna","umber","burnt sienna","mahogany",
"walnut","chestnut","hazel","bronze age","antique brass","patina","verdigris","seagrass","kelp","mangrove",
"coral blush","salmon","peach","apricot","mandarin","tangerine","pumpkin","copper glow","amber light","honeycomb",
"butterscotch","marigold","canary","dandelion","lemon","buttercup","custard","saffron","paprika","cardamom",
"emerald isle","jade pool","mint sprig","seafoam","oasis","cactus","sage","pickle","chartreuse","neon lawn",
"lapis","azurite","teal tide","cerulean","baltic","atlantic","pacific","indigo night","navy deep","marine",
"royal purple","imperial","violet dusk","orchid","lilac haze","wisteria","periwinkle","plum","mulberry","elderberry",
"lucent","gloss","matte","silk","satin","velvet","oxide","patina","flare","glint",
"gleam","spark","ember","glow","shine","sheen","polish","lustre","glisten","flicker",
"halo","aura","bloom","haze","veil","wash","tint","tone","shade","hue",
"tincture","pigment","chroma","spectrum","prism","refraction","iridescence","opalescent","lustrous pearl","shaded silver",
"gilded","bronzed","plated","forged","worn","vintage","aged","weathered","polished","brushed",
"patinated","oxidized","raw","pristine","clear","opaque","translucent","transparent","smoky","murky",
"crystalline","glassy","silvery blue","rosy dawn","dusky rose","sapphire dusk","golden hour","pearly dusk","silvered moon","inked night",
"verdant","blooming","floral","botanical","herbal","garden","meadow","pastel","soft","muted",
"vivid","brash","bold","electric","neon","fluorescent","lucid","clearcut","graphite","slate",
"storm","tempest","calm","serene","wild","tame","lush","sparse","dense","airy",
"thin","thick","rich","light","deep","bright","dull","brilliant","subdued","intense",
"muted glow","neon flash","soft dusk","harsh noon","evening chill","morning warmth","midnight frost","summer haze","winter glow","autumn fire",
"spring bloom","monsoon","dry season","harvest","boreal","tropic","polar","temperate","continental","maritime",
"continental dawn","coastal twilight","island morning","valley dusk","ridge morning","plateau noon","glen evening","hollow midnight","fen dawn","heath dusk",
"mesa glow","canyon hue","badlands","kiln red","potters clay","smoke stain","ink wash","linen dye","ocher tone","quartz pale"
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
"cinema glow","theatrical spot","stage wash","studio key","fill light","catch light","beauty light","rim flare","spotlight cold","spotlight warm","ambient evening","ambient morning","festive lights","fair lights","party glow",
"industrial haze","factory smog","tunnel gloom","subway glare","streetlamp glow","alley fog","neon alley","rainy neon","city mist","metropolis glow",
"countryside dawn","farmstead dusk","barn light","harvest haze","orchard light","vineyard dusk","lighthouse beam","coastal fog","harbor mist","ferry light",
"moonlit tide","salt wind","seabreeze","cliff fog","beach glow","dune haze","mirrored lagoon","lagoon glow","atoll light","reef shimmer",
"jungle canopy light","understory gloom","canopy shafts","rainforest mist","mangrove dusk","swamp sheen","bog fog","fen light","marsh glimmer","reed haze",
"highland mist","moor gloom","peat smoke","heath light","tarn shimmer","loch mist","stone circle dawn","stone circle dusk","barrow light","graveyard pall",
"crypt light","relic glow","relic haze","shrine glow","altar light","sacred hush","chant echo","procession torch","mandala glow","monk s dawn",
"temple dusk","pagoda light","bamboo shade","garden mist","bonsai glow","zen hush","meditation light","monastic twilight","hermit glow","oracle mist",
"sage smoke","incense haze","ritual flame","consecrated light","blessed gleam","cursed gleam","malefic shadow","baleful glow","sanguine light","nocturne hush",
"opera glow","gallery light","studio haze","atelier light","canvas sheen","pigment haze","brushstroke light","palette wash","gilded gallery","museum hush",
"laboratory glare","alchemical gleam","magus lamp","scientist s fluorescents","incubation glow","specimen light","cathode hum","circuit glimmer","device pulse","portal flare",
"portal aurora","rift shimmer","tear glow","breach light","rift fog","dimensional sheen","interstice glow","intersection light","junction mist","nexus shine",
"core hum","reactor glow","engine flare","keel light","bridge console glow","cockpit haze","starboard gleam","port shadow","anchor gloom","helm glow",
"voyager dawn","navigator dusk","cartographer s light","explorer s night","mariner s mist","astral haze","stellar shine","cosmic dust","vacuum glint","hubble glare",
"orbital dusk","satellite shimmer","lunar dust","sunspot glow","magnetosphere sheen","auroral curtains","ion trail","comet tail","meteoroid streak","event horizon",
"blackhole glare","singularity hush","time dilation shimmer","chronal haze","temporal ripple","past echo","future gleam","memory light","relic shimmer","remnant glow",
"heritage hush","ancestors veil","lineage light","legacy gleam","heirloom sheen","patina of time","rusted memory","aged glow","antique haze","vintage gleam",
"neon retro","synthwave haze","vaporwave glow","outrun shine","cyber dusk","techno flash","glitch shimmer","pixel glow","vector sheen","retro film",
"analog warmth","digital cold","chip light","PCB gleam","solder shine","lead trace","wire hum","relay flicker","switch click light","indicator glow",
"led wash","oled glow","plasma flare","tube warmth","cathode hum","zenith light","nadir shadow","equatorial glow","polar chill","tropical heat",
"temperate breeze","continental shift","oceanic swell","seismic tremor","tectonic rumble","faultline dust","volcanic plume","hot spring steam","geyser burst","magma shimmer",
"iron sky","steel mist","concrete haze","asphalt glare","glass reflection","chrome gleam","mirror sheen","polished stone glow","cairn hush","monolith shadow",
"tower glow","spire light","keep ember","moat mist","battlement breeze","sentry light","banner flutter","forge blaze","anvil sparks","bell tower chime",
"market bustle","merchant stall light","lantern alley","cobblestone shimmer","wagon dust","stable warmth","smithy heat","bakery steam","brewery haze","tavern glow",
"inn hearth","lamp post puddle","streetlamp halo","footpath dusk","laneway mist","courtyard light","arcade sparkle","fountain sheen","plaza noon","square dusk",
"festival lights","parade glow","carnival shine","fireworks burst","lantern release","paper lantern drift","string lights","bonfire blaze","drum circle vibe","midnight revel",
"quiet dawn","misty festival","empty stage","abandoned fair","lost song","sung silence","reverberant arch","echo chamber","hushed gallery","singular focus",
"meditative hush","chanting light","scripted glow","manual lantern","oil lamp glow","wick flame","match strike","spark flare","ember drift","coal bed",
"candle cluster","altar candlelight","chapel hush","basilica light","cathedral shadow","minster glow","cloister twilight","choir hush","vesper light","matins mist",
"serenity","tempest","resonance","reverie","suspense","revelation","mystery","clarity","obscurity","apotheosis"
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
"encaustic","fresco secco","sumi-e","nihonga","ukiyo-e","moku hanga","yangban","hanji","ink wash","sumi brush",
"paste paper","dip pen","speedpaint","painterly","brushwork","palette knife","impasto","glazing","sfumato","chiaroscuro",
"tenebrism","alla prima","grisaille","underpainting","wash","wet on wet","wet on dry","drybrush","scrumbling","glaze layer",
"line weight","contour line","cross hatching","stippling","scribble style","gesture drawing","figure study","anatomical study","fashion plate","editorial illustration",
"vector poster","glyphic","iconic","pictogram","emoji style","flat icon","material design","neumorphism","glassmorphism","skeuomorphism",
"retro computer","crt glow","scanline","dot matrix","vector graphics","early 3d","ray traced","path traced",
"photomanipulation","composite","double exposure","triptych","diptych","montage","posterize","solarize","negative","high key portrait",
"low key portrait","rim lit portrait","backlit portrait","environmental portrait","candid","studio portrait","street portrait","polished editorial","glamour","boudoir",
"advertising aesthetic","product catalog","ecommerce clean","lookbook","runway","high fashion","haute couture","avant garde","couture sketch","costume plate",
"fantastical","mythic realism","weirdcore","liminal space","aesthetic","anti-aesthetic","decayed beauty","ruin porn","sublime","grotesque",
"elegant","ornate","baroque flourish","rococo swirl","gothic silhouette","neo-gothic","vintage Victorian","Edwardian","regency","georgian",
"industrial","steampunk","brass and gears","rust and rivets","diesel age","art deco","art nouveau","mid century","atomic age","space age",
"brutalist","scandinavian minimal","japanese minimal","wabi sabi","organic modern","punk","grunge","underground zine","diary aesthetic","DIY",
"hand drawn","scribbled","marker art","felt pen","crayon","colored pencil","pastel","soft pastel","hard pastel","oil pastel",
"graphite","charcoal","conte","chalk","chalkboard","stencil","spray paint","marker pen","felt tip","fineliner",
"ink wash","pen and ink","dip ink","gouache","acrylic","acrylic pour","spray enamel","encaustic wax","latex paint","house paint",
"photoreal car","automotive render","product render","glass render","metal render","fabric render","fur render","hair render","skin shader","subsurface scatter",
"procedural texture","hand painted texture","tiling texture","seamless repeat","uv mapped","normal mapped","displacement","microdetails","wear and tear","aging",
"weathered","dented","scuffed","polished","brushed metal","oxidized finish","patina finish","lacquered","varnished","matte finish",
"glossy finish","satin finish","silky","sheer","textured","grainy","pitted","porous","smooth","velvet",
"linen","canvas","burlap","hessian","suede","leather","suede suede","denim","wool","cotton",
"silk","satin","velour","tweed","tartan","paisley","baroque pattern","damask","floral","geometric",
"tribal pattern","aztec","mayan","inca","celtic knot","norse runes","hieroglyphic","glyphic","ornamental","filigree",
"lacework","beaded","embroidery","applique","quilted","patched","stitched","woven","knitted","crocheted",
"handmade","artisan","craft","studio","atelier","workshop","forge","smith","luthier","carpenter",
"glassblower","ceramist","potter","weaver","dyer","tanner","cobbler","metalworker","stonecutter","stonemason",
"landscape","portrait","still life","abstract","conceptual","installation","performance","interactive","immersive","experiential",
"generative","procedural","ai assisted","neural style transfer","style mix","mashup","remix","reinterpretation","reimagined","reworked",
"collector s edition","archival","museum grade","gallery print","limited edition","open edition","signed print","artist proof","folio","portfolio",
"conceptual study","thumbnail sketch","final render","color study","value study","composition study","storyboard","animatic","frame study","keyframe",
"polished concept","pitch render","visual development","lookdev","shader study","lighting study","texture pass","model pass","rig test","final plate"
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
    
    // Add touch event handler to the canvas for touch-to-refresh - currently broken
    //lv_obj_add_flag(currentImage, LV_OBJ_FLAG_CLICKABLE);
    //lv_obj_add_event_cb(currentImage, touch_event_handler, LV_EVENT_CLICKED, NULL);
    
    bsp_display_unlock();
    
    Serial.println("=== SUCCESS - RGB565 Image displayed! ===");
    Serial.println("Touch screen to refresh or wait 2 minutes for auto-refresh\n");
    imageCount++;
}
