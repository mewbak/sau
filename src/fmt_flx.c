// Credits go to Exult/Pentagram team

#include "common.h"

static char *U71Shapes[1024];
static char *U72Shapes[1036];

static void initU7Shapes() {
  U71Shapes[0]="tiles/carpet";
  U71Shapes[1]="tiles/sidewalk";
  U71Shapes[2]="tiles/water";
  U71Shapes[3]="tiles/lightning";
  U71Shapes[4]="tiles/grass";
  U71Shapes[5]="tiles/cavefloor";
  U71Shapes[6]="tiles/arrow";
  U71Shapes[7]="tiles/spring";
  U71Shapes[8]="tiles/water";
  U71Shapes[9]="tiles/fireball";
  U71Shapes[10]="tiles/sand";
  U71Shapes[11]="tiles/ether";
  U71Shapes[12]="tiles/void";
  U71Shapes[13]="tiles/spring";
  U71Shapes[14]="tiles/ford";
  U71Shapes[15]="tiles/ford";
  U71Shapes[16]="tiles/rut";
  U71Shapes[17]="tiles/planking";
  U71Shapes[18]="tiles/tile";
  U71Shapes[19]="tiles/water";
  U71Shapes[20]="tiles/water";
  U71Shapes[21]="tiles/stone_floor";
  U71Shapes[22]="tiles/swamp";
  U71Shapes[23]="tiles/dirt";
  U71Shapes[24]="tiles/cobblestone";
  U71Shapes[25]="tiles/rut";
  U71Shapes[26]="tiles/water";
  U71Shapes[27]="tiles/carpet";
  U71Shapes[28]="tiles/sandy_grass";
  U71Shapes[29]="tiles/muddy_bank";
  U71Shapes[30]="tiles/water";
  U71Shapes[31]="tiles/grass";
  U71Shapes[32]="tiles/grass";
  U71Shapes[33]="tiles/grass";
  U71Shapes[34]="tiles/grass";
  U71Shapes[35]="tiles/grass";
  U71Shapes[36]="tiles/grass";
  U71Shapes[37]="tiles/grass";
  U71Shapes[38]="tiles/grass";
  U71Shapes[39]="tiles/grass";
  U71Shapes[40]="tiles/grass";
  U71Shapes[41]="tiles/grass";
  U71Shapes[42]="tiles/grass";
  U71Shapes[43]="tiles/grass";
  U71Shapes[44]="tiles/grass";
  U71Shapes[45]="tiles/grass";
  U71Shapes[46]="tiles/rocky_grass";
  U71Shapes[47]="tiles/carpet";
  U71Shapes[48]="tiles/ethereal_void";
  U71Shapes[49]="tiles/cavefloor";
  U71Shapes[50]="tiles/cavefloor";
  U71Shapes[51]="tiles/cavefloor";
  U71Shapes[52]="tiles/cavefloor";
  U71Shapes[53]="tiles/cavefloor";
  U71Shapes[54]="tiles/cavefloor";
  U71Shapes[55]="tiles/cavefloor";
  U71Shapes[56]="tiles/cavefloor";
  U71Shapes[57]="tiles/cavefloor";
  U71Shapes[58]="tiles/cavefloor";
  U71Shapes[59]="tiles/cavefloor";
  U71Shapes[60]="tiles/cavefloor";
  U71Shapes[61]="tiles/cavefloor";
  U71Shapes[62]="tiles/cavefloor";
  U71Shapes[63]="tiles/cavefloor";
  U71Shapes[64]="tiles/water";
  U71Shapes[65]="tiles/water";
  U71Shapes[66]="tiles/grass";
  U71Shapes[67]="tiles/muck";
  U71Shapes[68]="tiles/enchant_missiles";
  U71Shapes[69]="tiles/heal";
  U71Shapes[70]="tiles/light";
  U71Shapes[71]="tiles/destroy_trap";
  U71Shapes[72]="tiles/sleep";
  U71Shapes[73]="tiles/unlock_magic";
  U71Shapes[74]="tiles/light";
  U71Shapes[75]="tiles/dispel_field";
  U71Shapes[76]="tiles/heal";
  U71Shapes[77]="tiles/seance";
  U71Shapes[78]="tiles/explosion";
  U71Shapes[79]="tiles/reveal";
  U71Shapes[80]="tiles/charm";
  U71Shapes[81]="tiles/flamestrike";
  U71Shapes[82]="tiles/delayed_explosion";
  U71Shapes[83]="tiles/unnamed";
  U71Shapes[84]="tiles/ford";
  U71Shapes[85]="tiles/muddy_bank";
  U71Shapes[86]="tiles/muddy_bank";
  U71Shapes[87]="tiles/muddy_bank";
  U71Shapes[88]="tiles/muddy_bank";
  U71Shapes[89]="tiles/muddy_bank";
  U71Shapes[90]="tiles/muddy_bank";
  U71Shapes[91]="tiles/muddy_bank";
  U71Shapes[92]="tiles/muddy_bank";
  U71Shapes[93]="tiles/muddy_bank";
  U71Shapes[94]="tiles/muddy_bank";
  U71Shapes[95]="tiles/muddy_bank";
  U71Shapes[96]="tiles/muddy_bank";
  U71Shapes[97]="tiles/muddy_bank";
  U71Shapes[98]="tiles/muddy_bank";
  U71Shapes[99]="tiles/muddy_bank";
  U71Shapes[100]="tiles/muddy_bank";
  U71Shapes[101]="tiles/muddy_bank";
  U71Shapes[102]="tiles/muddy_bank";
  U71Shapes[103]="tiles/muddy_bank";
  U71Shapes[104]="tiles/muddy_bank";
  U71Shapes[105]="tiles/muddy_bank";
  U71Shapes[106]="tiles/muddy_bank";
  U71Shapes[107]="tiles/muddy_bank";
  U71Shapes[108]="tiles/muddy_bank";
  U71Shapes[109]="tiles/muddy_bank";
  U71Shapes[110]="tiles/unnamed";
  U71Shapes[111]="tiles/sandy_ground";
  U71Shapes[112]="tiles/ford";
  U71Shapes[113]="tiles/swamp";
  U71Shapes[114]="tiles/swamp";
  U71Shapes[115]="tiles/swamp";
  U71Shapes[116]="tiles/swamp";
  U71Shapes[117]="tiles/swamp";
  U71Shapes[118]="tiles/sandy_grass";
  U71Shapes[119]="tiles/sandy_grass";
  U71Shapes[120]="tiles/sandy_grass";
  U71Shapes[121]="tiles/sandy_grass";
  U71Shapes[122]="tiles/sandy_grass";
  U71Shapes[123]="tiles/sandy_grass";
  U71Shapes[124]="tiles/sandy_grass";
  U71Shapes[125]="tiles/sandy_grass";
  U71Shapes[126]="tiles/sandy_grass";
  U71Shapes[127]="tiles/sandy_grass";
  U71Shapes[128]="tiles/sandy_grass";
  U71Shapes[129]="tiles/sandy_grass";
  U71Shapes[130]="tiles/sandy_grass";
  U71Shapes[131]="tiles/sandy_grass";
  U71Shapes[132]="tiles/sandy_grass";
  U71Shapes[133]="tiles/sandy_grass";
  U71Shapes[134]="tiles/grassy_mud";
  U71Shapes[135]="tiles/grassy_mud";
  U71Shapes[136]="tiles/grassy_mud";
  U71Shapes[137]="tiles/grassy_mud";
  U71Shapes[138]="tiles/grassy_mud";
  U71Shapes[139]="tiles/grassy_mud";
  U71Shapes[140]="tiles/grassy_mud";
  U71Shapes[141]="tiles/grassy_mud";
  U71Shapes[142]="tiles/grassy_mud";
  U71Shapes[143]="tiles/grassy_mud";
  U71Shapes[144]="tiles/grassy_mud";
  U71Shapes[145]="tiles/grassy_mud";
  U71Shapes[146]="tiles/grass";
  U71Shapes[147]="tiles/grass";
  U71Shapes[148]="tiles/grass";
  U71Shapes[149]="tiles/mud";
  U71Shapes[150]="scenery/gangplank";
  U71Shapes[151]="walls/wall";
  U71Shapes[152]="walls/wall";
  U71Shapes[153]="scenery/oppressor";
  U71Shapes[154]="units/mage";
  U71Shapes[155]="units/ferryman";
  U71Shapes[156]="roofs/tile_roof";
  U71Shapes[157]="walls/Moongate";
  U71Shapes[158]="scenery/stand";
  U71Shapes[159]="scenery/pocketwatch";
  U71Shapes[160]="trees/greer_plant";
  U71Shapes[161]="roofs/unnamed";
  U71Shapes[162]="roofs/unnamed";
  U71Shapes[163]="scenery/rock_outcropping";
  U71Shapes[164]="roofs/slate_roof";
  U71Shapes[165]="roofs/slate_roof";
  U71Shapes[166]="roofs/slate_roof";
  U71Shapes[167]="roofs/slate_roof";
  U71Shapes[168]="projectiles/beam_of_light";
  U71Shapes[169]="roofs/slate_roof";
  U71Shapes[170]="roofs/wood_roof";
  U71Shapes[171]="roofs/wood_roof";
  U71Shapes[172]="roofs/wood_roof";
  U71Shapes[173]="roofs/wood_roof";
  U71Shapes[174]="roofs/wood_roof";
  U71Shapes[175]="roofs/wood_roof";
  U71Shapes[176]="roofs/wood_roof";
  U71Shapes[177]="scenery/alchemist_device";
  U71Shapes[178]="scenery/cloth_map";
  U71Shapes[179]="projectiles/lightning";
  U71Shapes[180]="tiles/mountain";
  U71Shapes[181]="trees/tree";
  U71Shapes[182]="tiles/mountain";
  U71Shapes[183]="tiles/mountain";
  U71Shapes[184]="tiles/caddelite_meteor";
  U71Shapes[185]="trees/dead_tree";
  U71Shapes[186]="tiles/carpet";
  U71Shapes[187]="tiles/carpet";
  U71Shapes[188]="tiles/rug";
  U71Shapes[189]="tiles/floor";
  U71Shapes[190]="tiles/carpet";
  U71Shapes[191]="walls/fortress";
  U71Shapes[192]="walls/fortress";
  U71Shapes[193]="tiles/floor";
  U71Shapes[194]="scenery/Knights_Bridge_piece";
  U71Shapes[195]="tiles/mountain";
  U71Shapes[196]="tiles/mountain";
  U71Shapes[197]="tiles/mountain";
  U71Shapes[198]="projectiles/light";
  U71Shapes[199]="scenery/mast";
  U71Shapes[200]="scenery/trap";
  U71Shapes[201]="scenery/debris";
  U71Shapes[202]="scenery/debris";
  U71Shapes[203]="scenery/small_rock";
  U71Shapes[204]="scenery/scorch_mark";
  U71Shapes[205]="walls/wall";
  U71Shapes[206]="walls/wall";
  U71Shapes[207]="walls/scorch_mark";
  U71Shapes[208]="walls/broken_door";
  U71Shapes[209]="scenery/dust";
  U71Shapes[210]="scenery/chicken_coop";
  U71Shapes[211]="walls/broken_door";
  U71Shapes[212]="walls/bridge";
  U71Shapes[213]="walls/bridge";
  U71Shapes[214]="walls/bridge";
  U71Shapes[215]="walls/bridge";
  U71Shapes[216]="walls/broken_wall";
  U71Shapes[217]="walls/broken_wall";
  U71Shapes[218]="walls/wall";
  U71Shapes[219]="walls/wall";
  U71Shapes[220]="walls/wall";
  U71Shapes[221]="walls/wall";
  U71Shapes[222]="scenery/blue_flag";
  U71Shapes[223]="roofs/broken_roof";
  U71Shapes[224]="projectiles/dust";
  U71Shapes[225]="walls/abbey_door";
  U71Shapes[226]="units/gargoyle_winged";
  U71Shapes[227]="units/alagner";
  U71Shapes[228]="units/papa";
  U71Shapes[229]="units/mama";
  U71Shapes[230]="units/ethereal_monster";
  U71Shapes[231]="scenery/blinker";
  U71Shapes[232]="scenery/red_flag";
  U71Shapes[233]="tiles/platform";
  U71Shapes[234]="tiles/Sphere_Generator";
  U71Shapes[235]="tiles/Sphere_Generator";
  U71Shapes[236]="tiles/Sphere_Generator";
  U71Shapes[237]="tiles/Sphere_Generator";
  U71Shapes[238]="tiles/Cube_Generator";
  U71Shapes[239]="tiles/Cube_Generator";
  U71Shapes[240]="tiles/Cube_Generator";
  U71Shapes[241]="tiles/Cube_Generator";
  U71Shapes[242]="tiles/Tetrahedron_Generator";
  U71Shapes[243]="tiles/Tetrahedron_Generator";
  U71Shapes[244]="tiles/Tetrahedron_Generator";
  U71Shapes[245]="tiles/Tetrahedron_Generator";
  U71Shapes[246]="walls/abbey_door";
  U71Shapes[247]="units/paladin";
  U71Shapes[248]="scenery/green_flag";
  U71Shapes[249]="scenery/top";
  U71Shapes[250]="walls/abbey_door";
  U71Shapes[251]="scenery/ship_sails";
  U71Shapes[252]="scenery/grandfather_clock";
  U71Shapes[253]="walls/wall";
  U71Shapes[254]="walls/wall";
  U71Shapes[255]="walls/broken_wall";
  U71Shapes[256]="tiles/delta";
  U71Shapes[257]="tiles/fortress_gateway";
  U71Shapes[258]="scenery/keg";
  U71Shapes[259]="units/fighter";
  U71Shapes[260]="walls/fortress";
  U71Shapes[261]="scenery/loom";
  U71Shapes[262]="scenery/unnamed";
  U71Shapes[263]="walls/fortress";
  U71Shapes[264]="scenery/iron_bars";
  U71Shapes[265]="units/townsman";
  U71Shapes[266]="walls/wall";
  U71Shapes[267]="scenery/mysterious_craft";
  U71Shapes[268]="scenery/mirror";
  U71Shapes[269]="tiles/carpet";
  U71Shapes[270]="walls/door";
  U71Shapes[271]="walls/portcullis";
  U71Shapes[272]="walls/portcullis";
  U71Shapes[273]="walls/wall";
  U71Shapes[274]="units/gargoyle_warrior";
  U71Shapes[275]="scenery/Egg";
  U71Shapes[276]="scenery/crossbeam";
  U71Shapes[277]="scenery/crossbeam";
  U71Shapes[278]="scenery/musket";
  U71Shapes[279]="walls/cavern";
  U71Shapes[280]="projectiles/ignite";
  U71Shapes[281]="projectiles/curse";
  U71Shapes[282]="scenery/painting";
  U71Shapes[283]="scenery/desk";
  U71Shapes[284]="scenery/sundial";
  U71Shapes[285]="scenery/cloak";
  U71Shapes[286]="scenery/banner";
  U71Shapes[287]="projectiles/swordstrike";
  U71Shapes[288]="projectiles/slime_attack";
  U71Shapes[289]="walls/The_Passion_Play_Theatre";
  U71Shapes[290]="walls/closed_shutters";
  U71Shapes[291]="walls/closed_shutters";
  U71Shapes[292]="scenery/seat";
  U71Shapes[293]="walls/tapestry";
  U71Shapes[294]="tiles/carpet";
  U71Shapes[295]="scenery/wedding_ring";
  U71Shapes[296]="scenery/ring_of_invisibility";
  U71Shapes[297]="scenery/ring_of_protection";
  U71Shapes[298]="scenery/ring_of_regeneration";
  U71Shapes[299]="units/ghost";
  U71Shapes[300]="scenery/cooking_utensils";
  U71Shapes[301]="scenery/cart";
  U71Shapes[302]="scenery/pumpkin";
  U71Shapes[303]="walls/metal";
  U71Shapes[304]="units/blacksmith";
  U71Shapes[305]="walls/The_Black_Gate";
  U71Shapes[306]="trees/evergreen";
  U71Shapes[307]="scenery/laboratory_burner";
  U71Shapes[308]="walls/wall";
  U71Shapes[309]="trees/fallen_tree";
  U71Shapes[310]="trees/tree";
  U71Shapes[311]="scenery/trophy";
  U71Shapes[312]="tiles/gargoyle_futon";
  U71Shapes[313]="trees/stump";
  U71Shapes[314]="trees/weeds";
  U71Shapes[315]="trees/fallen_tree";
  U71Shapes[316]="scenery/large_rock";
  U71Shapes[317]="units/ghost";
  U71Shapes[318]="units/sage";
  U71Shapes[319]="units/peasant";
  U71Shapes[320]="trees/brambles";
  U71Shapes[321]="trees/reeds";
  U71Shapes[322]="walls/open_shutters";
  U71Shapes[323]="trees/cattails";
  U71Shapes[324]="tiles/mountain";
  U71Shapes[325]="trees/dead_tree";
  U71Shapes[326]="trees/tropical_plant";
  U71Shapes[327]="trees/cypress_tree";
  U71Shapes[328]="trees/baobab_tree";
  U71Shapes[329]="scenery/kite";
  U71Shapes[330]="scenery/Virtue_Stone";
  U71Shapes[331]="scenery/rock";
  U71Shapes[332]="trees/tree";
  U71Shapes[333]="scenery/table";
  U71Shapes[334]="scenery/bubbles";
  U71Shapes[335]="scenery/bubbles";
  U71Shapes[336]="scenery/light_sources";
  U71Shapes[337]="units/ghost";
  U71Shapes[338]="scenery/light_sources_lit";
  U71Shapes[339]="projectiles/dispel_magic";
  U71Shapes[340]="scenery/potion";
  U71Shapes[341]="scenery/rock";
  U71Shapes[342]="scenery/boulder";
  U71Shapes[343]="scenery/boulder";
  U71Shapes[344]="walls/wall";
  U71Shapes[345]="walls/wall";
  U71Shapes[346]="walls/wall";
  U71Shapes[347]="walls/broken_wall";
  U71Shapes[348]="walls/wall";
  U71Shapes[349]="walls/wall";
  U71Shapes[350]="walls/wall";
  U71Shapes[351]="walls/wall";
  U71Shapes[352]="walls/fortress";
  U71Shapes[353]="scenery/greaves";
  U71Shapes[354]="units/liche";
  U71Shapes[355]="walls/wall";
  U71Shapes[356]="walls/broken_wall";
  U71Shapes[357]="walls/wall";
  U71Shapes[358]="walls/wall";
  U71Shapes[359]="walls/wall";
  U71Shapes[360]="scenery/sign";
  U71Shapes[361]="scenery/sign";
  U71Shapes[362]="walls/wall";
  U71Shapes[363]="tiles/gargoyle_futon";
  U71Shapes[364]="tiles/platform";
  U71Shapes[365]="walls/wall";
  U71Shapes[366]="walls/wall";
  U71Shapes[367]="tiles/floor";
  U71Shapes[368]="tiles/floor";
  U71Shapes[369]="tiles/floor";
  U71Shapes[370]="tiles/floor";
  U71Shapes[371]="walls/wall";
  U71Shapes[372]="walls/open_shutters";
  U71Shapes[373]="walls/glass_wall";
  U71Shapes[374]="walls/wall";
  U71Shapes[375]="units/unicorn";
  U71Shapes[376]="walls/door";
  U71Shapes[377]="scenery/food_item";
  U71Shapes[378]="scenery/fence";
  U71Shapes[379]="scenery/sign";
  U71Shapes[380]="units/cyclope";
  U71Shapes[381]="units/hydra";
  U71Shapes[382]="units/kissme";
  U71Shapes[383]="scenery/magic_helm";
  U71Shapes[384]="scenery/waves";
  U71Shapes[385]="walls/stairs";
  U71Shapes[386]="walls/stairs";
  U71Shapes[387]="walls/stairs";
  U71Shapes[388]="scenery/eating_utensils";
  U71Shapes[389]="walls/cavern";
  U71Shapes[390]="walls/cavern";
  U71Shapes[391]="walls/cavern";
  U71Shapes[392]="walls/abbey_door";
  U71Shapes[393]="walls/wall";
  U71Shapes[394]="units/guard";
  U71Shapes[395]="tiles/mountain";
  U71Shapes[396]="tiles/mountain";
  U71Shapes[397]="projectiles/unnamed";
  U71Shapes[398]="projectiles/unnamed";
  U71Shapes[399]="projectiles/energy_mist";
  U71Shapes[400]="units/body";
  U71Shapes[401]="units/pirate";
  U71Shapes[402]="tiles/ferry";
  U71Shapes[403]="units/batlin";
  U71Shapes[404]="scenery/honeycomb";
  U71Shapes[405]="tiles/ships_hold";
  U71Shapes[406]="scenery/nightstand";
  U71Shapes[407]="scenery/desk";
  U71Shapes[408]="projectiles/paralyze";
  U71Shapes[409]="scenery/trophy";
  U71Shapes[410]="scenery/mining_machine";
  U71Shapes[411]="scenery/conveyor_belt";
  U71Shapes[412]="tiles/black_gate_dais";
  U71Shapes[413]="tiles/carpet";
  U71Shapes[414]="units/body";
  U71Shapes[415]="scenery/garbage";
  U71Shapes[416]="scenery/drawers";
  U71Shapes[417]="projectiles/magic_bolt";
  U71Shapes[418]="scenery/giant_bones";
  U71Shapes[419]="tiles/delta";
  U71Shapes[420]="walls/fence";
  U71Shapes[421]="walls/fence";
  U71Shapes[422]="walls/fence";
  U71Shapes[423]="trees/crops";
  U71Shapes[424]="projectiles/poison";
  U71Shapes[425]="walls/wall";
  U71Shapes[426]="walls/stairs";
  U71Shapes[427]="walls/stairs";
  U71Shapes[428]="walls/stairs";
  U71Shapes[429]="walls/stairs";
  U71Shapes[430]="walls/stairs";
  U71Shapes[431]="scenery/bellows";
  U71Shapes[432]="walls/door";
  U71Shapes[433]="walls/door";
  U71Shapes[434]="scenery/cask";
  U71Shapes[435]="scenery/lit_sconce";
  U71Shapes[436]="scenery/wagon_floor";
  U71Shapes[437]="scenery/wagon_wheel";
  U71Shapes[438]="scenery/window";
  U71Shapes[439]="roofs/chimney";
  U71Shapes[440]="projectiles/light";
  U71Shapes[441]="tiles/floor";
  U71Shapes[442]="scenery/fireplace";
  U71Shapes[443]="projectiles/telekinesis";
  U71Shapes[444]="scenery/hood";
  U71Shapes[445]="units/mage";
  U71Shapes[446]="units/mage";
  U71Shapes[447]="units/wounded_man";
  U71Shapes[448]="units/sage";
  U71Shapes[449]="units/beggar";
  U71Shapes[450]="units/beggar";
  U71Shapes[451]="units/noble";
  U71Shapes[452]="units/peasant";
  U71Shapes[453]="trees/tree";
  U71Shapes[454]="units/shopkeeper";
  U71Shapes[455]="units/shopkeeper";
  U71Shapes[456]="units/noble";
  U71Shapes[457]="units/gypsy";
  U71Shapes[458]="units/pirate";
  U71Shapes[459]="units/wench";
  U71Shapes[460]="units/ranger";
  U71Shapes[461]="units/ranger";
  U71Shapes[462]="units/fighter";
  U71Shapes[463]="units/fighter";
  U71Shapes[464]="units/paladin";
  U71Shapes[465]="units/iolo";
  U71Shapes[466]="units/lord_british";
  U71Shapes[467]="units/jester";
  U71Shapes[468]="units/entertainer";
  U71Shapes[469]="units/entertainer";
  U71Shapes[470]="scenery/well";
  U71Shapes[471]="units/child";
  U71Shapes[472]="units/child";
  U71Shapes[473]="units/gargoyle_noble";
  U71Shapes[474]="scenery/sling";
  U71Shapes[475]="units/gargoyle_worker";
  U71Shapes[476]="units/smith_the_horse";
  U71Shapes[477]="units/frank_the_fox";
  U71Shapes[478]="units/sherry_the_mouse";
  U71Shapes[479]="units/emp";
  U71Shapes[480]="units/draxinusom";
  U71Shapes[481]="scenery/sconce";
  U71Shapes[482]="units/batlin";
  U71Shapes[483]="tiles/rug";
  U71Shapes[484]="walls/bars";
  U71Shapes[485]="units/highwayman";
  U71Shapes[486]="units/statue";
  U71Shapes[487]="units/shamino";
  U71Shapes[488]="units/dupre";
  U71Shapes[489]="units/spark";
  U71Shapes[490]="units/jaana";
  U71Shapes[491]="units/acid_slug";
  U71Shapes[492]="units/alligator";
  U71Shapes[493]="units/bat";
  U71Shapes[494]="units/bee";
  U71Shapes[495]="units/cat";
  U71Shapes[496]="units/dog";
  U71Shapes[497]="walls/wall";
  U71Shapes[498]="units/chicken";
  U71Shapes[499]="units/corpser";
  U71Shapes[500]="units/cow";
  U71Shapes[501]="units/cyclope";
  U71Shapes[502]="units/deer";
  U71Shapes[503]="walls/puppet_show";
  U71Shapes[504]="units/dragon";
  U71Shapes[505]="units/drake";
  U71Shapes[506]="units/hook";
  U71Shapes[507]="units/corpse";
  U71Shapes[508]="walls/stained_glass_window";
  U71Shapes[509]="units/fish";
  U71Shapes[510]="units/fox";
  U71Shapes[511]="units/gazer";
  U71Shapes[512]="walls/stained_glass_window";
  U71Shapes[513]="units/gremlin";
  U71Shapes[514]="units/headless";
  U71Shapes[515]="walls/closed_drawbridge";
  U71Shapes[516]="tiles/delta";
  U71Shapes[517]="units/insects";
  U71Shapes[518]="tiles/glass_counter_tops";
  U71Shapes[519]="scenery/liche";
  U71Shapes[520]="scenery/Virtue_Roulette_Table";
  U71Shapes[521]="units/mouse";
  U71Shapes[522]="scenery/locked_chest";
  U71Shapes[523]="units/rat";
  U71Shapes[524]="units/reaper";
  U71Shapes[525]="units/sea_serpent";
  U71Shapes[526]="scenery/lit_lamp";
  U71Shapes[527]="projectiles/death_bolt";
  U71Shapes[528]="units/skeleton";
  U71Shapes[529]="units/slime";
  U71Shapes[530]="units/snake";
  U71Shapes[531]="walls/cavern";
  U71Shapes[532]="units/harpie";
  U71Shapes[533]="units/troll";
  U71Shapes[534]="units/wisp";
  U71Shapes[535]="scenery/spent_sconce";
  U71Shapes[536]="units/tentacles";
  U71Shapes[537]="units/wolf";
  U71Shapes[538]="walls/fence";
  U71Shapes[539]="scenery/chain_coif";
  U71Shapes[540]="projectiles/douse";
  U71Shapes[541]="scenery/great_helm";
  U71Shapes[542]="scenery/crested_helm";
  U71Shapes[543]="scenery/buckler";
  U71Shapes[544]="scenery/broken_columns";
  U71Shapes[545]="scenery/curved_heater";
  U71Shapes[546]="scenery/broken_dish";
  U71Shapes[547]="projectiles/magic_sword";
  U71Shapes[548]="projectiles/hoe_of_destruction";
  U71Shapes[549]="scenery/lightning_whip";
  U71Shapes[550]="projectiles/magic_boomerang";
  U71Shapes[551]="projectiles/fire_sword";
  U71Shapes[552]="projectiles/magic_axe";
  U71Shapes[553]="projectiles/firedoom_staff";
  U71Shapes[554]="projectiles/burst_arrows";
  U71Shapes[555]="units/hawk";
  U71Shapes[556]="projectiles/magic_arrows";
  U71Shapes[557]="projectiles/juggernaut_hammer";
  U71Shapes[558]="projectiles/lucky_arrows";
  U71Shapes[559]="projectiles/magebane";
  U71Shapes[560]="projectiles/love_arrows";
  U71Shapes[561]="projectiles/great_dagger";
  U71Shapes[562]="projectiles/death_scythe";
  U71Shapes[563]="projectiles/blowgun";
  U71Shapes[564]="projectiles/poison_dagger";
  U71Shapes[565]="projectiles/starbursts";
  U71Shapes[566]="projectiles/glimmer";
  U71Shapes[567]="projectiles/sword_of_defense";
  U71Shapes[568]="projectiles/tseramed_arrows";
  U71Shapes[569]="scenery/leather_armour";
  U71Shapes[570]="scenery/scale_armour";
  U71Shapes[571]="scenery/chain_armour";
  U71Shapes[572]="scenery/wooden_shield";
  U71Shapes[573]="scenery/plate_armour";
  U71Shapes[574]="scenery/leather_leggings";
  U71Shapes[575]="scenery/chain_leggings";
  U71Shapes[576]="scenery/plate_leggings";
  U71Shapes[577]="scenery/pedestal";
  U71Shapes[578]="scenery/spiked_shield";
  U71Shapes[579]="scenery/leather_gloves";
  U71Shapes[580]="scenery/gauntlets";
  U71Shapes[581]="projectiles/ammunition";
  U71Shapes[582]="scenery/leather_collar";
  U71Shapes[583]="scenery/bed_roll";
  U71Shapes[584]="scenery/kidney_belt";
  U71Shapes[585]="scenery/shoes";
  U71Shapes[586]="scenery/gorget";
  U71Shapes[587]="scenery/boots";
  U71Shapes[588]="scenery/swamp_boots";
  U71Shapes[589]="projectiles/pitchfork";
  U71Shapes[590]="projectiles/club";
  U71Shapes[591]="projectiles/main_gauche";
  U71Shapes[592]="projectiles/spears";
  U71Shapes[593]="projectiles/throwing_axes";
  U71Shapes[594]="projectiles/daggers";
  U71Shapes[595]="projectiles/torch";
  U71Shapes[596]="projectiles/morning_star";
  U71Shapes[597]="projectiles/bow";
  U71Shapes[598]="projectiles/crossbow";
  U71Shapes[599]="projectiles/sword";
  U71Shapes[600]="projectiles/two_handed_hammer";
  U71Shapes[601]="projectiles/two_handed_axe";
  U71Shapes[602]="projectiles/two_handed_sword";
  U71Shapes[603]="projectiles/halberd";
  U71Shapes[604]="projectiles/glass_sword";
  U71Shapes[605]="projectiles/boomerang";
  U71Shapes[606]="projectiles/magic_bow";
  U71Shapes[607]="scenery/path";
  U71Shapes[608]="projectiles/decorative_sword";
  U71Shapes[609]="scenery/kite_shield";
  U71Shapes[610]="tiles/delta";
  U71Shapes[611]="trees/lily_pads";
  U71Shapes[612]="tiles/surf";
  U71Shapes[613]="tiles/surf";
  U71Shapes[614]="projectiles/cleaver";
  U71Shapes[615]="projectiles/knives";
  U71Shapes[616]="scenery/bottle";
  U71Shapes[617]="units/time_lord";
  U71Shapes[618]="projectiles/scythe";
  U71Shapes[619]="trees/fern";
  U71Shapes[620]="projectiles/rake";
  U71Shapes[621]="projectiles/delayed_explosion";
  U71Shapes[622]="projectiles/whip";
  U71Shapes[623]="projectiles/hammers";
  U71Shapes[624]="projectiles/pick";
  U71Shapes[625]="projectiles/shovel";
  U71Shapes[626]="projectiles/hoe";
  U71Shapes[627]="scenery/lockpicks";
  U71Shapes[628]="scenery/cup";
  U71Shapes[629]="projectiles/lightning_wand";
  U71Shapes[630]="projectiles/fire_wand";
  U71Shapes[631]="trees/tree";
  U71Shapes[632]="tiles/surf";
  U71Shapes[633]="scenery/table";
  U71Shapes[634]="scenery/standing_stone";
  U71Shapes[635]="projectiles/custom_sword";
  U71Shapes[636]="projectiles/serpentine_daggers";
  U71Shapes[637]="projectiles/serpentine_sword";
  U71Shapes[638]="scenery/caddellite_helmet";
  U71Shapes[639]="projectiles/death_vortex";
  U71Shapes[640]="scenery/ring";
  U71Shapes[641]="scenery/keys";
  U71Shapes[642]="scenery/book";
  U71Shapes[643]="trees/tree";
  U71Shapes[644]="scenery/gold_coins";
  U71Shapes[645]="scenery/gold_nuggets";
  U71Shapes[646]="scenery/gold_bars";
  U71Shapes[647]="projectiles/triple_crossbow";
  U71Shapes[648]="scenery/sleeping_powder";
  U71Shapes[649]="scenery/venom";
  U71Shapes[650]="scenery/sextant";
  U71Shapes[651]="scenery/spinning_wheel";
  U71Shapes[652]="scenery/cart";
  U71Shapes[653]="scenery/bale_of_wool";
  U71Shapes[654]="scenery/spindle_of_thread";
  U71Shapes[655]="units/topiary";
  U71Shapes[656]="scenery/wine_press";
  U71Shapes[657]="walls/curtain";
  U71Shapes[658]="scenery/dough";
  U71Shapes[659]="projectiles/mace";
  U71Shapes[660]="scenery/cart";
  U71Shapes[661]="units/mongbat";
  U71Shapes[662]="projectiles/fishing_rod";
  U71Shapes[663]="scenery/magic_shield";
  U71Shapes[664]="scenery/stove";
  U71Shapes[665]="scenery/ship_prow";
  U71Shapes[666]="scenery/magic_armor";
  U71Shapes[667]="scenery/cobwebs";
  U71Shapes[668]="projectiles/sword_blank";
  U71Shapes[669]="trees/cactus";
  U71Shapes[670]="trees/maple_tree";
  U71Shapes[671]="trees/mushrooms";
  U71Shapes[672]="scenery/shrubbery";
  U71Shapes[673]="trees/plant";
  U71Shapes[674]="trees/silverleaf_tree";
  U71Shapes[675]="scenery/desk_item";
  U71Shapes[676]="projectiles/unnamed";
  U71Shapes[677]="scenery/sack_of_wheat";
  U71Shapes[678]="walls/curtain";
  U71Shapes[679]="scenery/drawers";
  U71Shapes[680]="scenery/deck";
  U71Shapes[681]="scenery/jar";
  U71Shapes[682]="scenery/glass_item";
  U71Shapes[683]="trees/tree";
  U71Shapes[684]="scenery/trellis";
  U71Shapes[685]="scenery/mechanism";
  U71Shapes[686]="scenery/magic_leggings";
  U71Shapes[687]="scenery/pillar";
  U71Shapes[688]="scenery/blank_type";
  U71Shapes[689]="scenery/harpsichord";
  U71Shapes[690]="scenery/xylophone";
  U71Shapes[691]="scenery/lyre";
  U71Shapes[692]="scenery/lute";
  U71Shapes[693]="scenery/whistle";
  U71Shapes[694]="walls/cave_door";
  U71Shapes[695]="scenery/grandfather_clock";
  U71Shapes[696]="scenery/bed";
  U71Shapes[697]="scenery/podium";
  U71Shapes[698]="scenery/shears";
  U71Shapes[699]="tiles/surf";
  U71Shapes[700]="tiles/deck";
  U71Shapes[701]="scenery/lit_torch";
  U71Shapes[702]="scenery/cannon";
  U71Shapes[703]="scenery/cannon_ball";
  U71Shapes[704]="scenery/powder_keg";
  U71Shapes[705]="scenery/rope_ladder";
  U71Shapes[706]="units/scorpion";
  U71Shapes[707]="projectiles/black_sword";
  U71Shapes[708]="scenery/stocks";
  U71Shapes[709]="scenery/guillotine";
  U71Shapes[710]="scenery/log_saw";
  U71Shapes[711]="scenery/mill_stone";
  U71Shapes[712]="scenery/stove";
  U71Shapes[713]="scenery/post";
  U71Shapes[714]="scenery/rope_ladder";
  U71Shapes[715]="scenery/tombstone";
  U71Shapes[716]="units/bird";
  U71Shapes[717]="scenery/plate";
  U71Shapes[718]="scenery/pedestal";
  U71Shapes[719]="scenery/water_trough";
  U71Shapes[720]="units/guard";
  U71Shapes[721]="units/avatar";
  U71Shapes[722]="projectiles/arrows";
  U71Shapes[723]="projectiles/bolts";
  U71Shapes[724]="scenery/fellowship_icon";
  U71Shapes[725]="scenery/virtue_roulette_wheel";
  U71Shapes[726]="scenery/lens";
  U71Shapes[727]="units/horse";
  U71Shapes[728]="scenery/caddellite_chunks";
  U71Shapes[729]="scenery/crystal_ball";
  U71Shapes[730]="scenery/baby";
  U71Shapes[731]="projectiles/unnamed";
  U71Shapes[732]="walls/window";
  U71Shapes[733]="tiles/carpet";
  U71Shapes[734]="scenery/guillotine";
  U71Shapes[735]="scenery/archery_target";
  U71Shapes[736]="tiles/surf";
  U71Shapes[737]="tiles/surf";
  U71Shapes[738]="scenery/pants";
  U71Shapes[739]="scenery/firepit";
  U71Shapes[740]="scenery/well";
  U71Shapes[741]="scenery/water_trough";
  U71Shapes[742]="scenery/stuffed_toy";
  U71Shapes[743]="scenery/strength_tester";
  U71Shapes[744]="scenery/telescope";
  U71Shapes[745]="scenery/harp";
  U71Shapes[746]="scenery/orrery_crystal";
  U71Shapes[747]="scenery/soul_cage";
  U71Shapes[748]="scenery/well_of_souls";
  U71Shapes[749]="scenery/alchemist_aparatus";
  U71Shapes[750]="tiles/carpet";
  U71Shapes[751]="tiles/surf";
  U71Shapes[752]="scenery/music_box";
  U71Shapes[753]="units/stone_harpie";
  U71Shapes[754]="scenery/vial";
  U71Shapes[755]="tiles/orrery_paths";
  U71Shapes[756]="scenery/caltrops";
  U71Shapes[757]="scenery/cart";
  U71Shapes[758]="tiles/carpet";
  U71Shapes[759]="scenery/ethereal_ring";
  U71Shapes[760]="scenery/gem";
  U71Shapes[761]="scenery/spellbook";
  U71Shapes[762]="units/body";
  U71Shapes[763]="scenery/rat_race_game";
  U71Shapes[764]="scenery/rat_race_game";
  U71Shapes[765]="scenery/planet_britannia";
  U71Shapes[766]="scenery/moon_trammel";
  U71Shapes[767]="scenery/small_orrery";
  U71Shapes[768]="scenery/energy_field";
  U71Shapes[769]="scenery/smokebombs";
  U71Shapes[770]="scenery/orrery_viewer";
  U71Shapes[771]="scenery/Rudyoms_Wand";
  U71Shapes[772]="scenery/honey";
  U71Shapes[773]="scenery/cart";
  U71Shapes[774]="scenery/cart";
  U71Shapes[775]="scenery/ship_rails";
  U71Shapes[776]="scenery/moongate";
  U71Shapes[777]="scenery/moongate";
  U71Shapes[778]="units/body";
  U71Shapes[779]="scenery/moongate";
  U71Shapes[780]="scenery/bubbles";
  U71Shapes[781]="scenery/gang_plank";
  U71Shapes[782]="projectiles/flaming_oil";
  U71Shapes[783]="trees/tree";
  U71Shapes[784]="units/emp";
  U71Shapes[785]="scenery/orb_of_the_moons";
  U71Shapes[786]="scenery/vortex_cube";
  U71Shapes[787]="scenery/lever";
  U71Shapes[788]="scenery/switch";
  U71Shapes[789]="scenery/sword_in_a_stone";
  U71Shapes[790]="scenery/invisibility_dust";
  U71Shapes[791]="scenery/ship";
  U71Shapes[792]="projectiles/magicians_wand";
  U71Shapes[793]="tiles/pit";
  U71Shapes[794]="scenery/water_objects";
  U71Shapes[795]="scenery/mantle";
  U71Shapes[796]="units/draft_horse";
  U71Shapes[797]="scenery/scroll";
  U71Shapes[798]="scenery/sealed_box";
  U71Shapes[799]="scenery/unsealed_box";
  U71Shapes[800]="scenery/chest";
  U71Shapes[801]="scenery/backpack";
  U71Shapes[802]="scenery/bag";
  U71Shapes[803]="scenery/basket";
  U71Shapes[804]="scenery/crate";
  U71Shapes[805]="units/forskis";
  U71Shapes[806]="units/guard";
  U71Shapes[807]="projectiles/lightning";
  U71Shapes[808]="tiles/surf";
  U71Shapes[809]="scenery/The_Triples_Game";
  U71Shapes[810]="scenery/bucket";
  U71Shapes[811]="units/rabbit";
  U71Shapes[812]="scenery/iron_maiden";
  U71Shapes[813]="scenery/rack";
  U71Shapes[814]="scenery/triples_game";
  U71Shapes[815]="scenery/stone_chips";
  U71Shapes[816]="scenery/nest";
  U71Shapes[817]="trees/cactus";
  U71Shapes[818]="scenery/triples_game";
  U71Shapes[819]="scenery/barrel";
  U71Shapes[820]="scenery/plaque";
  U71Shapes[821]="scenery/painting";
  U71Shapes[822]="scenery/diaper";
  U71Shapes[823]="scenery/artists_equipment";
  U71Shapes[824]="scenery/pitcher";
  U71Shapes[825]="scenery/campfire";
  U71Shapes[826]="trees/tree_top";
  U71Shapes[827]="scenery/bandages";
  U71Shapes[828]="walls/wall";
  U71Shapes[829]="tiles/floor";
  U71Shapes[830]="scenery/gear";
  U71Shapes[831]="scenery/baking_hearth";
  U71Shapes[832]="walls/glass_wall";
  U71Shapes[833]="walls/glass_wall";
  U71Shapes[834]="tiles/surf";
  U71Shapes[835]="scenery/magic_gauntlets";
  U71Shapes[836]="scenery/antique_armor";
  U71Shapes[837]="scenery/easel";
  U71Shapes[838]="scenery/avatar_costume";
  U71Shapes[839]="scenery/hourglass";
  U71Shapes[840]="scenery/flying_carpet";
  U71Shapes[841]="walls/glass_wall";
  U71Shapes[842]="scenery/reagents";
  U71Shapes[843]="scenery/magic_gorget";
  U71Shapes[844]="units/sea_serpent";
  U71Shapes[845]="walls/wall";
  U71Shapes[846]="scenery/mantle";
  U71Shapes[847]="scenery/table";
  U71Shapes[848]="scenery/mirror";
  U71Shapes[849]="walls/tapestry";
  U71Shapes[850]="tiles/carpet";
  U71Shapes[851]="scenery/cloth";
  U71Shapes[852]="walls/crenellations";
  U71Shapes[853]="scenery/greenhouse_roof";
  U71Shapes[854]="units/statue";
  U71Shapes[855]="scenery/rats_face_cage";
  U71Shapes[856]="projectiles/fire_bolt";
  U71Shapes[857]="projectiles/dragon_breath";
  U71Shapes[858]="scenery/bookshelf";
  U71Shapes[859]="projectiles/casting_frames";
  U71Shapes[860]="scenery/fencing_dummy";
  U71Shapes[861]="units/troll";
  U71Shapes[862]="scenery/shaft";
  U71Shapes[863]="scenery/kitchen_items";
  U71Shapes[864]="units/toddler";
  U71Shapes[865]="units/spider";
  U71Shapes[866]="scenery/thumbscrews";
  U71Shapes[867]="units/victim";
  U71Shapes[868]="scenery/wall_mount";
  U71Shapes[869]="walls/wall";
  U71Shapes[870]="walls/drawbridge";
  U71Shapes[871]="walls/wall";
  U71Shapes[872]="scenery/stove";
  U71Shapes[873]="scenery/chair";
  U71Shapes[874]="tiles/cave";
  U71Shapes[875]="tiles/surf";
  U71Shapes[876]="walls/metal";
  U71Shapes[877]="scenery/Rune";
  U71Shapes[878]="walls/crenellations";
  U71Shapes[879]="walls/wall";
  U71Shapes[880]="scenery/shaft";
  U71Shapes[881]="units/elizabeth";
  U71Shapes[882]="units/abraham";
  U71Shapes[883]="units/gargoyle_wingless";
  U71Shapes[884]="units/fellowship_member";
  U71Shapes[885]="projectiles/fellowship_staff";
  U71Shapes[886]="scenery/anchor";
  U71Shapes[887]="scenery/test_tubes";
  U71Shapes[888]="scenery/horn";
  U71Shapes[889]="scenery/lamp_post";
  U71Shapes[890]="scenery/table";
  U71Shapes[891]="roofs/wood_roof";
  U71Shapes[892]="units/body";
  U71Shapes[893]="scenery/pool_of_water";
  U71Shapes[894]="scenery/monolith";
  U71Shapes[895]="projectiles/fire_field";
  U71Shapes[896]="walls/crenellations";
  U71Shapes[897]="scenery/seat";
  U71Shapes[898]="scenery/altar";
  U71Shapes[899]="walls/wall";
  U71Shapes[900]="projectiles/poison_field";
  U71Shapes[901]="walls/crenellations";
  U71Shapes[902]="projectiles/sleep_field";
  U71Shapes[903]="walls/crenellations";
  U71Shapes[904]="walls/wall";
  U71Shapes[905]="walls/cavern";
  U71Shapes[906]="units/Ssatue";
  U71Shapes[907]="tiles/surf";
  U71Shapes[908]="roofs/tile_roof";
  U71Shapes[909]="walls/wall";
  U71Shapes[910]="walls/wall";
  U71Shapes[911]="tiles/surf";
  U71Shapes[912]="scenery/blood";
  U71Shapes[913]="scenery/pillar";
  U71Shapes[914]="scenery/blackrock";
  U71Shapes[915]="scenery/chunks_of_lead";
  U71Shapes[916]="scenery/chunks_of_iron_ore";
  U71Shapes[917]="scenery/shaft";
  U71Shapes[918]="tiles/surf";
  U71Shapes[919]="scenery/bookshelf";
  U71Shapes[920]="scenery/pillar";
  U71Shapes[921]="scenery/betting_marker";
  U71Shapes[922]="trees/small_bush";
  U71Shapes[923]="scenery/piece_of_wood";
  U71Shapes[924]="walls/cavern";
  U71Shapes[925]="walls/cavern";
  U71Shapes[926]="tiles/delta";
  U71Shapes[927]="tiles/delta";
  U71Shapes[928]="walls/cavern";
  U71Shapes[929]="units/fellowship_member";
  U71Shapes[930]="tiles/delta";
  U71Shapes[931]="tiles/mountain";
  U71Shapes[932]="trees/tree";
  U71Shapes[933]="scenery/millsaw";
  U71Shapes[934]="scenery/water_wheel";
  U71Shapes[935]="scenery/portcullis_door";
  U71Shapes[936]="scenery/portcullis_door";
  U71Shapes[937]="scenery/gargoyle_jewelry";
  U71Shapes[938]="tiles/delta";
  U71Shapes[939]="walls/wall";
  U71Shapes[940]="walls/wall";
  U71Shapes[941]="scenery/water_wheel";
  U71Shapes[942]="scenery/water_wheel";
  U71Shapes[943]="walls/wall";
  U71Shapes[944]="scenery/pot";
  U71Shapes[945]="units/guardian_statue";
  U71Shapes[946]="units/guard";
  U71Shapes[947]="scenery/bee_stinger";
  U71Shapes[948]="projectiles/bolt";
  U71Shapes[949]="scenery/winch";
  U71Shapes[950]="scenery/winch";
  U71Shapes[951]="scenery/water_wheel";
  U71Shapes[952]="units/ferryman";
  U71Shapes[953]="walls/wall";
  U71Shapes[954]="roofs/greenhouse_roof";
  U71Shapes[955]="scenery/amulet";
  U71Shapes[956]="roofs/wood_roof";
  U71Shapes[957]="units/barkeep";
  U71Shapes[958]="scenery/moon_felucca";
  U71Shapes[959]="tiles/mountain";
  U71Shapes[960]="trees/mushroom";
  U71Shapes[961]="scenery/barge";
  U71Shapes[962]="roofs/slate_roof";
  U71Shapes[963]="roofs/slate_roof";
  U71Shapes[964]="scenery/table";
  U71Shapes[965]="units/child";
  U71Shapes[966]="roofs/tile_roof";
  U71Shapes[967]="walls/broken_wall";
  U71Shapes[968]="scenery/enclosed_prism";
  U71Shapes[969]="tiles/mountain";
  U71Shapes[970]="units/sheep";
  U71Shapes[971]="scenery/table";
  U71Shapes[972]="walls/wall";
  U71Shapes[973]="walls/stairs";
  U71Shapes[974]="walls/stairs";
  U71Shapes[975]="scenery/wagon_door";
  U71Shapes[976]="scenery/wagon";
  U71Shapes[977]="scenery/wagon";
  U71Shapes[978]="scenery/wagon";
  U71Shapes[979]="scenery/wagon_roof";
  U71Shapes[980]="walls/wall";
  U71Shapes[981]="scenery/prism";
  U71Shapes[982]="roofs/chimney";
  U71Shapes[983]="tiles/mountain";
  U71Shapes[984]="scenery/piece_of_wood";
  U71Shapes[985]="projectiles/waves";
  U71Shapes[986]="scenery/stove_top";
  U71Shapes[987]="scenery/baby_in_a_cradle";
  U71Shapes[988]="scenery/planet";
  U71Shapes[989]="units/avatar";
  U71Shapes[990]="scenery/dark_core";
  U71Shapes[991]="scenery/anvil";
  U71Shapes[992]="scenery/cradle";
  U71Shapes[993]="trees/mushroom";
  U71Shapes[994]="scenery/tongs";
  U71Shapes[995]="scenery/cauldron";
  U71Shapes[996]="walls/glass_wall";
  U71Shapes[997]="scenery/light_source_used";
  U71Shapes[998]="scenery/horseshoe";
  U71Shapes[999]="scenery/plant";
  U71Shapes[1000]="scenery/table";
  U71Shapes[1001]="scenery/table";
  U71Shapes[1002]="scenery/table";
  U71Shapes[1003]="scenery/table";
  U71Shapes[1004]="scenery/leather_helm";
  U71Shapes[1005]="walls/glass_wall";
  U71Shapes[1006]="walls/broken_wall";
  U71Shapes[1007]="walls/glass_wall";
  U71Shapes[1008]="projectiles/waves";
  U71Shapes[1009]="projectiles/waves";
  U71Shapes[1010]="scenery/prism";
  U71Shapes[1011]="scenery/bed";
  U71Shapes[1012]="tiles/surf";
  U71Shapes[1013]="scenery/rocking_horse";
  U71Shapes[1014]="tiles/floor";
  U71Shapes[1015]="units/golem";
  U71Shapes[1016]="tiles/floor";
  U71Shapes[1017]="scenery/ship";
  U71Shapes[1018]="scenery/table";
  U71Shapes[1019]="walls/glass_wall";
  U71Shapes[1020]="tiles/surf";
  U71Shapes[1021]="scenery/time_barrier";
  U71Shapes[1022]="tiles/surf";
  U71Shapes[1023]="scenery/haystack";

  U72Shapes[0]="tiles/water";
  U72Shapes[1]="tiles/sidewalk";
  U72Shapes[2]="tiles/unnamed";
  U72Shapes[3]="tiles/icy_water";
  U72Shapes[4]="tiles/floor";
  U72Shapes[5]="tiles/cavefloor";
  U72Shapes[6]="tiles/icy_water";
  U72Shapes[7]="tiles/spring";
  U72Shapes[8]="tiles/water";
  U72Shapes[9]="tiles/ice_ice_baby";
  U72Shapes[10]="tiles/sand";
  U72Shapes[11]="tiles/ice";
  U72Shapes[12]="tiles/void";
  U72Shapes[13]="tiles/spring";
  U72Shapes[14]="tiles/floor";
  U72Shapes[15]="tiles/tile_floor";
  U72Shapes[16]="tiles/ice";
  U72Shapes[17]="tiles/planking";
  U72Shapes[18]="tiles/tile";
  U72Shapes[19]="tiles/unnamed";
  U72Shapes[20]="tiles/unnamed";
  U72Shapes[21]="tiles/stone_floor";
  U72Shapes[22]="tiles/swamp";
  U72Shapes[23]="tiles/dirt";
  U72Shapes[24]="tiles/cobblestone";
  U72Shapes[25]="tiles/ice";
  U72Shapes[26]="tiles/unnamed";
  U72Shapes[27]="tiles/carpet";
  U72Shapes[28]="tiles/sandy_grass";
  U72Shapes[29]="tiles/muddy_bank";
  U72Shapes[30]="tiles/unnamed";
  U72Shapes[31]="tiles/snow";
  U72Shapes[32]="tiles/snow";
  U72Shapes[33]="tiles/ice";
  U72Shapes[34]="tiles/floor";
  U72Shapes[35]="tiles/damage_causing_cell";
  U72Shapes[36]="tiles/wood_floor";
  U72Shapes[37]="tiles/Serpent_Highway";
  U72Shapes[38]="tiles/tile_floor";
  U72Shapes[39]="tiles/floor";
  U72Shapes[40]="tiles/floor";
  U72Shapes[41]="tiles/grass";
  U72Shapes[42]="tiles/icy_floor";
  U72Shapes[43]="tiles/icy_floor";
  U72Shapes[44]="tiles/icy_floor";
  U72Shapes[45]="tiles/icy_floor";
  U72Shapes[46]="tiles/icy_floor";
  U72Shapes[47]="tiles/carpet";
  U72Shapes[48]="tiles/ethereal_void";
  U72Shapes[49]="tiles/cavefloor";
  U72Shapes[50]="tiles/cavefloor";
  U72Shapes[51]="tiles/cavefloor";
  U72Shapes[52]="tiles/da_floor";
  U72Shapes[53]="tiles/cavefloor";
  U72Shapes[54]="tiles/cavefloor";
  U72Shapes[55]="tiles/cavefloor";
  U72Shapes[56]="tiles/cavefloor";
  U72Shapes[57]="tiles/cavefloor";
  U72Shapes[58]="tiles/cavefloor";
  U72Shapes[59]="tiles/cavefloor";
  U72Shapes[60]="tiles/cavefloor";
  U72Shapes[61]="tiles/cavefloor";
  U72Shapes[62]="tiles/cavefloor";
  U72Shapes[63]="tiles/floor";
  U72Shapes[64]="tiles/unnamed";
  U72Shapes[65]="tiles/unnamed";
  U72Shapes[66]="tiles/grass";
  U72Shapes[67]="tiles/muck";
  U72Shapes[68]="tiles/icy_floor";
  U72Shapes[69]="tiles/icy_floor";
  U72Shapes[70]="tiles/chasm_wall";
  U72Shapes[71]="tiles/chasm_wall";
  U72Shapes[72]="tiles/dirt";
  U72Shapes[73]="tiles/dirt";
  U72Shapes[74]="tiles/wood_floor";
  U72Shapes[75]="tiles/unnamed";
  U72Shapes[76]="tiles/unnamed";
  U72Shapes[77]="tiles/unnamed";
  U72Shapes[78]="tiles/grass";
  U72Shapes[79]="tiles/rocky_grass";
  U72Shapes[80]="tiles/a_long_fall";
  U72Shapes[81]="tiles/a_long_fall";
  U72Shapes[82]="tiles/unnamed";
  U72Shapes[83]="tiles/unnamed";
  U72Shapes[84]="tiles/unnamed";
  U72Shapes[85]="tiles/muddy_bank";
  U72Shapes[86]="tiles/muddy_bank";
  U72Shapes[87]="tiles/muddy_bank";
  U72Shapes[88]="tiles/muddy_bank";
  U72Shapes[89]="tiles/muddy_bank";
  U72Shapes[90]="tiles/muddy_bank";
  U72Shapes[91]="tiles/muddy_bank";
  U72Shapes[92]="tiles/muddy_bank";
  U72Shapes[93]="tiles/muddy_bank";
  U72Shapes[94]="tiles/muddy_bank";
  U72Shapes[95]="tiles/muddy_bank";
  U72Shapes[96]="tiles/muddy_bank";
  U72Shapes[97]="tiles/muddy_bank";
  U72Shapes[98]="tiles/muddy_bank";
  U72Shapes[99]="tiles/muddy_bank";
  U72Shapes[100]="tiles/muddy_bank";
  U72Shapes[101]="tiles/muddy_bank";
  U72Shapes[102]="tiles/muddy_bank";
  U72Shapes[103]="tiles/muddy_bank";
  U72Shapes[104]="tiles/muddy_bank";
  U72Shapes[105]="tiles/muddy_bank";
  U72Shapes[106]="tiles/muddy_bank";
  U72Shapes[107]="tiles/muddy_bank";
  U72Shapes[108]="tiles/muddy_bank";
  U72Shapes[109]="tiles/muddy_bank";
  U72Shapes[110]="tiles/unnamed";
  U72Shapes[111]="tiles/sandy_ground";
  U72Shapes[112]="tiles/unnamed";
  U72Shapes[113]="tiles/swamp";
  U72Shapes[114]="tiles/swamp";
  U72Shapes[115]="tiles/swamp";
  U72Shapes[116]="tiles/swamp";
  U72Shapes[117]="tiles/swamp";
  U72Shapes[118]="tiles/sandy_grass";
  U72Shapes[119]="tiles/sandy_grass";
  U72Shapes[120]="tiles/sandy_grass";
  U72Shapes[121]="tiles/sandy_grass";
  U72Shapes[122]="tiles/sandy_grass";
  U72Shapes[123]="tiles/sandy_grass";
  U72Shapes[124]="tiles/sandy_grass";
  U72Shapes[125]="tiles/sandy_grass";
  U72Shapes[126]="tiles/sandy_grass";
  U72Shapes[127]="tiles/sandy_grass";
  U72Shapes[128]="tiles/sandy_grass";
  U72Shapes[129]="tiles/sandy_grass";
  U72Shapes[130]="tiles/sandy_grass";
  U72Shapes[131]="tiles/sandy_grass";
  U72Shapes[132]="tiles/sandy_grass";
  U72Shapes[133]="tiles/sandy_grass";
  U72Shapes[134]="tiles/grassy_mud";
  U72Shapes[135]="tiles/grassy_mud";
  U72Shapes[136]="tiles/grassy_mud";
  U72Shapes[137]="tiles/grassy_mud";
  U72Shapes[138]="tiles/grassy_mud";
  U72Shapes[139]="tiles/grassy_mud";
  U72Shapes[140]="tiles/grassy_mud";
  U72Shapes[141]="tiles/grassy_mud";
  U72Shapes[142]="tiles/grassy_mud";
  U72Shapes[143]="tiles/grassy_mud";
  U72Shapes[144]="tiles/grassy_mud";
  U72Shapes[145]="tiles/grassy_mud";
  U72Shapes[146]="tiles/grass";
  U72Shapes[147]="tiles/grass";
  U72Shapes[148]="tiles/grass";
  U72Shapes[149]="tiles/mud";
  U72Shapes[150]="scenery/gang_plank";
  U72Shapes[151]="walls/illusion";
  U72Shapes[152]="walls/illusion";
  U72Shapes[153]="scenery/fountain";
  U72Shapes[154]="trees/grass";
  U72Shapes[155]="walls/door_frame";
  U72Shapes[156]="roofs/tile_roof";
  U72Shapes[157]="walls/door_frame";
  U72Shapes[158]="walls/wall";
  U72Shapes[159]="projectiles/flamer";
  U72Shapes[160]="tiles/serpent_gate";
  U72Shapes[161]="roofs/thatch_roof";
  U72Shapes[162]="roofs/thatch_roof";
  U72Shapes[163]="scenery/rock_outcropping";
  U72Shapes[164]="roofs/slate_roof";
  U72Shapes[165]="roofs/slate_roof";
  U72Shapes[166]="roofs/slate_roof";
  U72Shapes[167]="roofs/slate_roof";
  U72Shapes[168]="tiles/mountain";
  U72Shapes[169]="roofs/slate_roof";
  U72Shapes[170]="roofs/wood_roof";
  U72Shapes[171]="roofs/wood_roof";
  U72Shapes[172]="roofs/wood_roof";
  U72Shapes[173]="roofs/wood_roof";
  U72Shapes[174]="roofs/wood_roof";
  U72Shapes[175]="roofs/wood_roof";
  U72Shapes[176]="walls/wall";
  U72Shapes[177]="tiles/lava";
  U72Shapes[178]="scenery/cloth_map";
  U72Shapes[179]="units/monk";
  U72Shapes[180]="tiles/mountain";
  U72Shapes[181]="trees/tree";
  U72Shapes[182]="tiles/mountain";
  U72Shapes[183]="tiles/mountain";
  U72Shapes[184]="tiles/lava";
  U72Shapes[185]="trees/dead_tree";
  U72Shapes[186]="tiles/carpet";
  U72Shapes[187]="tiles/carpet";
  U72Shapes[188]="tiles/rug";
  U72Shapes[189]="tiles/floor";
  U72Shapes[190]="tiles/carpet";
  U72Shapes[191]="walls/fortress";
  U72Shapes[192]="walls/fortress";
  U72Shapes[193]="tiles/floor";
  U72Shapes[194]="tiles/ground";
  U72Shapes[195]="tiles/mountain";
  U72Shapes[196]="tiles/mountain";
  U72Shapes[197]="tiles/mountain";
  U72Shapes[198]="projectiles/light";
  U72Shapes[199]="scenery/mast";
  U72Shapes[200]="scenery/trap";
  U72Shapes[201]="scenery/debris";
  U72Shapes[202]="scenery/debris";
  U72Shapes[203]="scenery/small_rock";
  U72Shapes[204]="projectiles/scorch_mark";
  U72Shapes[205]="walls/wall";
  U72Shapes[206]="walls/wall";
  U72Shapes[207]="walls/scorch_mark";
  U72Shapes[208]="walls/broken_door";
  U72Shapes[209]="scenery/unnamed";
  U72Shapes[210]="scenery/chicken_coop";
  U72Shapes[211]="walls/broken_door";
  U72Shapes[212]="walls/bridge";
  U72Shapes[213]="walls/bridge";
  U72Shapes[214]="units/gwani";
  U72Shapes[215]="tiles/turtle";
  U72Shapes[216]="walls/broken_wall";
  U72Shapes[217]="walls/broken_wall";
  U72Shapes[218]="walls/wall";
  U72Shapes[219]="tiles/turtle";
  U72Shapes[220]="walls/wall";
  U72Shapes[221]="tiles/turtle";
  U72Shapes[222]="scenery/pennant";
  U72Shapes[223]="roofs/broken_roof";
  U72Shapes[224]="projectiles/unnamed";
  U72Shapes[225]="walls/door";
  U72Shapes[226]="units/mage";
  U72Shapes[227]="scenery/cloak";
  U72Shapes[228]="units/pikeman";
  U72Shapes[229]="units/man";
  U72Shapes[230]="units/woman";
  U72Shapes[231]="projectiles/magebane_sword";
  U72Shapes[232]="walls/ice_wall";
  U72Shapes[233]="tiles/platform";
  U72Shapes[234]="tiles/ice";
  U72Shapes[235]="tiles/drift";
  U72Shapes[236]="tiles/mountain";
  U72Shapes[237]="tiles/drift";
  U72Shapes[238]="trees/tree";
  U72Shapes[239]="scenery/unnamed";
  U72Shapes[240]="tiles/stairs";
  U72Shapes[241]="projectiles/staff";
  U72Shapes[242]="scenery/saw";
  U72Shapes[243]="scenery/coffin";
  U72Shapes[244]="scenery/large_skull";
  U72Shapes[245]="scenery/bear_skeleton";
  U72Shapes[246]="walls/door";
  U72Shapes[247]="units/jester";
  U72Shapes[248]="scenery/ship_prow";
  U72Shapes[249]="scenery/oracle";
  U72Shapes[250]="units/monk";
  U72Shapes[251]="scenery/ship_sails";
  U72Shapes[252]="tiles/turtle";
  U72Shapes[253]="walls/wall";
  U72Shapes[254]="walls/wall";
  U72Shapes[255]="walls/broken_wall";
  U72Shapes[256]="walls/wall";
  U72Shapes[257]="walls/fortress_gateway";
  U72Shapes[258]="scenery/keg";
  U72Shapes[259]="units/ranger";
  U72Shapes[260]="walls/fortress";
  U72Shapes[261]="scenery/loom";
  U72Shapes[262]="scenery/blackrock_serpent";
  U72Shapes[263]="walls/fortress";
  U72Shapes[264]="trees/pinecone";
  U72Shapes[265]="units/townsman";
  U72Shapes[266]="walls/wall";
  U72Shapes[267]="scenery/pumice";
  U72Shapes[268]="scenery/mirror";
  U72Shapes[269]="tiles/carpet";
  U72Shapes[270]="walls/door";
  U72Shapes[271]="walls/portcullis";
  U72Shapes[272]="walls/portcullis";
  U72Shapes[273]="walls/wall";
  U72Shapes[274]="units/woman";
  U72Shapes[275]="scenery/egg";
  U72Shapes[276]="scenery/crossbeam";
  U72Shapes[277]="scenery/crossbeam";
  U72Shapes[278]="scenery/twigs";
  U72Shapes[279]="walls/cavern";
  U72Shapes[280]="projectiles/ignite";
  U72Shapes[281]="projectiles/curse";
  U72Shapes[282]="walls/painting";
  U72Shapes[283]="scenery/desk";
  U72Shapes[284]="scenery/sundial";
  U72Shapes[285]="scenery/brush";
  U72Shapes[286]="scenery/drum";
  U72Shapes[287]="projectiles/swordstrike";
  U72Shapes[288]="projectiles/slime_attack";
  U72Shapes[289]="projectiles/fire";
  U72Shapes[290]="walls/shutters";
  U72Shapes[291]="walls/shutters";
  U72Shapes[292]="scenery/seat";
  U72Shapes[293]="walls/tapestry";
  U72Shapes[294]="tiles/carpet";
  U72Shapes[295]="tiles/mountain";
  U72Shapes[296]="scenery/ring";
  U72Shapes[297]="trees/hollow_tree";
  U72Shapes[298]="units/penguin";
  U72Shapes[299]="units/cantra";
  U72Shapes[300]="scenery/cooking_utensil";
  U72Shapes[301]="projectiles/barrier";
  U72Shapes[302]="units/bear";
  U72Shapes[303]="walls/sliding_door";
  U72Shapes[304]="units/man";
  U72Shapes[305]="scenery/serpent_statue";
  U72Shapes[306]="trees/evergreen";
  U72Shapes[307]="scenery/burner";
  U72Shapes[308]="walls/wall";
  U72Shapes[309]="trees/fallen_tree";
  U72Shapes[310]="trees/tree";
  U72Shapes[311]="scenery/trophy";
  U72Shapes[312]="units/ghoul";
  U72Shapes[313]="trees/stump";
  U72Shapes[314]="trees/weeds";
  U72Shapes[315]="trees/fallen_tree";
  U72Shapes[316]="scenery/large_rock";
  U72Shapes[317]="units/anti_shamino";
  U72Shapes[318]="units/batlin";
  U72Shapes[319]="units/peasant";
  U72Shapes[320]="trees/brambles";
  U72Shapes[321]="trees/reeds";
  U72Shapes[322]="scenery/basin";
  U72Shapes[323]="trees/cattails";
  U72Shapes[324]="tiles/mountain";
  U72Shapes[325]="trees/dead_tree";
  U72Shapes[326]="scenery/fountain";
  U72Shapes[327]="trees/cypress_tree";
  U72Shapes[328]="trees/baobab_tree";
  U72Shapes[329]="units/boar";
  U72Shapes[330]="scenery/serpent_rune";
  U72Shapes[331]="scenery/rock";
  U72Shapes[332]="tiles/rug";
  U72Shapes[333]="scenery/table";
  U72Shapes[334]="units/wildman";
  U72Shapes[335]="tiles/bubbles";
  U72Shapes[336]="scenery/light_source";
  U72Shapes[337]="units/ghost";
  U72Shapes[338]="scenery/light_source_lit";
  U72Shapes[339]="scenery/dispel_magic";
  U72Shapes[340]="projectiles/potion";
  U72Shapes[341]="scenery/rock";
  U72Shapes[342]="scenery/boulder";
  U72Shapes[343]="scenery/boulder";
  U72Shapes[344]="walls/wall";
  U72Shapes[345]="walls/wall";
  U72Shapes[346]="walls/wall";
  U72Shapes[347]="walls/broken_wall";
  U72Shapes[348]="walls/wall";
  U72Shapes[349]="walls/wall";
  U72Shapes[350]="walls/wall";
  U72Shapes[351]="walls/wall";
  U72Shapes[352]="walls/fortress";
  U72Shapes[353]="walls/wall";
  U72Shapes[354]="units/goblin_king";
  U72Shapes[355]="walls/wall";
  U72Shapes[356]="walls/broken_wall";
  U72Shapes[357]="walls/wall";
  U72Shapes[358]="walls/wall";
  U72Shapes[359]="walls/wall";
  U72Shapes[360]="scenery/sign";
  U72Shapes[361]="scenery/sign";
  U72Shapes[362]="scenery/wall";
  U72Shapes[363]="units/magic_gremlin";
  U72Shapes[364]="tiles/platform";
  U72Shapes[365]="walls/wall";
  U72Shapes[366]="walls/wall";
  U72Shapes[367]="tiles/floor";
  U72Shapes[368]="tiles/floor";
  U72Shapes[369]="tiles/floor";
  U72Shapes[370]="tiles/floor";
  U72Shapes[371]="walls/wall";
  U72Shapes[372]="units/ice_troll";
  U72Shapes[373]="units/mummy";
  U72Shapes[374]="walls/wall";
  U72Shapes[375]="units/unicorn";
  U72Shapes[376]="walls/door";
  U72Shapes[377]="scenery/food_item";
  U72Shapes[378]="scenery/fence";
  U72Shapes[379]="scenery/sign";
  U72Shapes[380]="walls/wall_of_lights";
  U72Shapes[381]="units/fawn_guard";
  U72Shapes[382]="units/ratman";
  U72Shapes[383]="scenery/magic_helm";
  U72Shapes[384]="projectiles/waves";
  U72Shapes[385]="walls/stairs";
  U72Shapes[386]="walls/stairs";
  U72Shapes[387]="walls/stairs";
  U72Shapes[388]="tiles/floor";
  U72Shapes[389]="walls/cavern";
  U72Shapes[390]="walls/cavern";
  U72Shapes[391]="walls/cavern";
  U72Shapes[392]="units/xenka";
  U72Shapes[393]="walls/wall";
  U72Shapes[394]="roofs/roof";
  U72Shapes[395]="tiles/mountain";
  U72Shapes[396]="tiles/mountain";
  U72Shapes[397]="projectiles/unnamed";
  U72Shapes[398]="projectiles/unnamed";
  U72Shapes[399]="projectiles/energy_mist";
  U72Shapes[400]="units/body";
  U72Shapes[401]="units/mage";
  U72Shapes[402]="units/body";
  U72Shapes[403]="scenery/cloak";
  U72Shapes[404]="scenery/food";
  U72Shapes[405]="scenery/ships_hold";
  U72Shapes[406]="projectiles/nightstand";
  U72Shapes[407]="scenery/desk";
  U72Shapes[408]="scenery/paralyze";
  U72Shapes[409]="scenery/trophy";
  U72Shapes[410]="scenery/saw";
  U72Shapes[411]="walls/wall";
  U72Shapes[412]="scenery/fountain";
  U72Shapes[413]="tiles/carpet";
  U72Shapes[414]="units/body";
  U72Shapes[415]="scenery/garbage";
  U72Shapes[416]="scenery/drawers";
  U72Shapes[417]="projectiles/magic_bolt";
  U72Shapes[418]="walls/wall";
  U72Shapes[419]="scenery/breast_plate";
  U72Shapes[420]="walls/fence";
  U72Shapes[421]="walls/fence";
  U72Shapes[422]="walls/fence";
  U72Shapes[423]="trees/stalks";
  U72Shapes[424]="projectiles/poison";
  U72Shapes[425]="walls/wall";
  U72Shapes[426]="walls/stairs";
  U72Shapes[427]="walls/stairs";
  U72Shapes[428]="walls/stairs";
  U72Shapes[429]="walls/stairs";
  U72Shapes[430]="walls/stairs";
  U72Shapes[431]="scenery/bellows";
  U72Shapes[432]="walls/door";
  U72Shapes[433]="walls/door";
  U72Shapes[434]="scenery/cask";
  U72Shapes[435]="scenery/lit_sconce";
  U72Shapes[436]="tiles/mountain";
  U72Shapes[437]="tiles/mountain";
  U72Shapes[438]="walls/window";
  U72Shapes[439]="walls/wall";
  U72Shapes[440]="projectiles/light";
  U72Shapes[441]="tiles/floor";
  U72Shapes[442]="walls/fireplace";
  U72Shapes[443]="projectiles/telekinesis";
  U72Shapes[444]="tiles/mountain";
  U72Shapes[445]="scenery/soul_prism";
  U72Shapes[446]="units/ugly_old_woman";
  U72Shapes[447]="units/timberwolf";
  U72Shapes[448]="tiles/mountain";
  U72Shapes[449]="units/bear";
  U72Shapes[450]="scenery/orb";
  U72Shapes[451]="units/noble";
  U72Shapes[452]="scenery/wand";
  U72Shapes[453]="trees/tree";
  U72Shapes[454]="units/shopkeeper";
  U72Shapes[455]="units/shopkeeper";
  U72Shapes[456]="scenery/flux_analyzer";
  U72Shapes[457]="units/gypsy";
  U72Shapes[458]="units/pirate";
  U72Shapes[459]="tiles/turtle";
  U72Shapes[460]="units/skeletal_dragon";
  U72Shapes[461]="units/ranger";
  U72Shapes[462]="units/fighter";
  U72Shapes[463]="tiles/floor";
  U72Shapes[464]="units/paladin";
  U72Shapes[465]="units/iolo";
  U72Shapes[466]="tiles/mountain";
  U72Shapes[467]="scenery/magic_plants";
  U72Shapes[468]="units/stranger";
  U72Shapes[469]="units/woman";
  U72Shapes[470]="scenery/well";
  U72Shapes[471]="walls/door";
  U72Shapes[472]="units/child";
  U72Shapes[473]="walls/wall_of_lights";
  U72Shapes[474]="scenery/magic_sling";
  U72Shapes[475]="walls/wall_of_lights";
  U72Shapes[476]="scenery/wand";
  U72Shapes[477]="tiles/mountain";
  U72Shapes[478]="units/goblin";
  U72Shapes[479]="scenery/unnamed";
  U72Shapes[480]="units/gargoyle";
  U72Shapes[481]="scenery/sconce";
  U72Shapes[482]="units/ghost";
  U72Shapes[483]="tiles/rug";
  U72Shapes[484]="walls/bars";
  U72Shapes[485]="scenery/unnamed";
  U72Shapes[486]="scenery/usecode_container";
  U72Shapes[487]="units/shamino";
  U72Shapes[488]="units/dupre";
  U72Shapes[489]="scenery/rod";
  U72Shapes[490]="scenery/dupres_shield";
  U72Shapes[491]="units/acid_slug";
  U72Shapes[492]="units/alligator";
  U72Shapes[493]="units/bat";
  U72Shapes[494]="units/undead";
  U72Shapes[495]="units/cat";
  U72Shapes[496]="units/dog";
  U72Shapes[497]="walls/wall";
  U72Shapes[498]="units/chicken";
  U72Shapes[499]="units/corpser";
  U72Shapes[500]="units/cow";
  U72Shapes[501]="units/cyclope";
  U72Shapes[502]="units/deer";
  U72Shapes[503]="tiles/barge";
  U72Shapes[504]="tiles/ice_raft";
  U72Shapes[505]="scenery/magic_lens";
  U72Shapes[506]="scenery/magic_harp";
  U72Shapes[507]="units/corpse";
  U72Shapes[508]="projectiles/hammer";
  U72Shapes[509]="units/fish";
  U72Shapes[510]="units/fox";
  U72Shapes[511]="units/gazer";
  U72Shapes[512]="units/worm";
  U72Shapes[513]="tiles/floor";
  U72Shapes[514]="units/headless";
  U72Shapes[515]="walls/drawbridge";
  U72Shapes[516]="walls/door";
  U72Shapes[517]="units/insects";
  U72Shapes[518]="scenery/glass_counter_top";
  U72Shapes[519]="scenery/moons_eye";
  U72Shapes[520]="projectiles/wooden_sword";
  U72Shapes[521]="units/dream_creature";
  U72Shapes[522]="scenery/locked_chest";
  U72Shapes[523]="units/rat";
  U72Shapes[524]="units/reaper";
  U72Shapes[525]="scenery/dust";
  U72Shapes[526]="scenery/lit_lamp";
  U72Shapes[527]="projectiles/death_bolt";
  U72Shapes[528]="units/skeleton";
  U72Shapes[529]="units/slime";
  U72Shapes[530]="units/snake";
  U72Shapes[531]="walls/cavern";
  U72Shapes[532]="units/harpie";
  U72Shapes[533]="units/troll";
  U72Shapes[534]="walls/unnamed";
  U72Shapes[535]="projectiles/dragonslayer_sword";
  U72Shapes[536]="units/tentacles";
  U72Shapes[537]="units/wolf";
  U72Shapes[538]="walls/fence";
  U72Shapes[539]="scenery/chain_coif";
  U72Shapes[540]="projectiles/douse";
  U72Shapes[541]="scenery/great_helm";
  U72Shapes[542]="scenery/crested_helm";
  U72Shapes[543]="scenery/buckler";
  U72Shapes[544]="scenery/broken_column";
  U72Shapes[545]="scenery/curved_heater";
  U72Shapes[546]="scenery/broken_dish";
  U72Shapes[547]="projectiles/magic_sword";
  U72Shapes[548]="scenery/sceptre";
  U72Shapes[549]="projectiles/lightning_whip";
  U72Shapes[550]="units/suit_of_armour";
  U72Shapes[551]="projectiles/fire_sword";
  U72Shapes[552]="projectiles/magic_axe";
  U72Shapes[553]="projectiles/firedoom_staff";
  U72Shapes[554]="projectiles/burst_arrow";
  U72Shapes[555]="scenery/serpent_jawbone";
  U72Shapes[556]="projectiles/magic_arrow";
  U72Shapes[557]="projectiles/juggernaut_hammer";
  U72Shapes[558]="projectiles/lucky_arrow";
  U72Shapes[559]="scenery/serpent_tooth";
  U72Shapes[560]="units/priestess";
  U72Shapes[561]="projectiles/blue_flame";
  U72Shapes[562]="scenery/bane_jail";
  U72Shapes[563]="projectiles/blowgun";
  U72Shapes[564]="projectiles/poison_dagger";
  U72Shapes[565]="units/fighter";
  U72Shapes[566]="projectiles/glimmer";
  U72Shapes[567]="projectiles/sword_of_defense";
  U72Shapes[568]="projectiles/sleep_arrow";
  U72Shapes[569]="scenery/leather_armour";
  U72Shapes[570]="scenery/scale_armour";
  U72Shapes[571]="scenery/chain_armour";
  U72Shapes[572]="scenery/wooden_shield";
  U72Shapes[573]="scenery/plate_armour";
  U72Shapes[574]="scenery/leather_leggings";
  U72Shapes[575]="scenery/chain_leggings";
  U72Shapes[576]="scenery/plate_leggings";
  U72Shapes[577]="scenery/nest";
  U72Shapes[578]="scenery/spiked_shield";
  U72Shapes[579]="scenery/gloves";
  U72Shapes[580]="scenery/gauntlets";
  U72Shapes[581]="units/old_man";
  U72Shapes[582]="walls/catwalk";
  U72Shapes[583]="scenery/bed_roll";
  U72Shapes[584]="scenery/kidney_belt";
  U72Shapes[585]="scenery/door_shield";
  U72Shapes[586]="scenery/brass_shield";
  U72Shapes[587]="scenery/boots";
  U72Shapes[588]="units/vampire";
  U72Shapes[589]="projectiles/pitchfork";
  U72Shapes[590]="projectiles/club";
  U72Shapes[591]="projectiles/serpent_arrow";
  U72Shapes[592]="projectiles/spear";
  U72Shapes[593]="projectiles/throwing_axe";
  U72Shapes[594]="projectiles/dagger";
  U72Shapes[595]="projectiles/torch";
  U72Shapes[596]="projectiles/morning_star";
  U72Shapes[597]="projectiles/bow";
  U72Shapes[598]="projectiles/crossbow";
  U72Shapes[599]="projectiles/sword";
  U72Shapes[600]="projectiles/two_handed_hammer";
  U72Shapes[601]="projectiles/two_handed_axe";
  U72Shapes[602]="projectiles/two_handed_sword";
  U72Shapes[603]="projectiles/halberd";
  U72Shapes[604]="projectiles/glass_sword";
  U72Shapes[605]="projectiles/boomerang";
  U72Shapes[606]="projectiles/magic_bow";
  U72Shapes[607]="scenery/path";
  U72Shapes[608]="projectiles/decorative_sword";
  U72Shapes[609]="scenery/kite_shield";
  U72Shapes[610]="scenery/scale";
  U72Shapes[611]="scenery/lily_pads";
  U72Shapes[612]="tiles/surf";
  U72Shapes[613]="tiles/surf";
  U72Shapes[614]="scenery/magic_music_player";
  U72Shapes[615]="walls/Wall_of_Lights";
  U72Shapes[616]="scenery/bottle";
  U72Shapes[617]="scenery/well";
  U72Shapes[618]="projectiles/scythe";
  U72Shapes[619]="walls/catwalk";
  U72Shapes[620]="projectiles/rake";
  U72Shapes[621]="projectiles/delayed_explosion";
  U72Shapes[622]="projectiles/whip";
  U72Shapes[623]="projectiles/hammer";
  U72Shapes[624]="projectiles/pick";
  U72Shapes[625]="projectiles/shovel";
  U72Shapes[626]="projectiles/hoe";
  U72Shapes[627]="scenery/lockpick";
  U72Shapes[628]="scenery/cup";
  U72Shapes[629]="projectiles/lightning_wand";
  U72Shapes[630]="projectiles/fire_wand";
  U72Shapes[631]="trees/tree";
  U72Shapes[632]="tiles/surf";
  U72Shapes[633]="scenery/table";
  U72Shapes[634]="scenery/standing_stone";
  U72Shapes[635]="scenery/Serpent_Earrings";
  U72Shapes[636]="projectiles/serpent_dagger";
  U72Shapes[637]="projectiles/serpent_sword";
  U72Shapes[638]="scenery/Serpent_Armour";
  U72Shapes[639]="projectiles/death_vortex";
  U72Shapes[640]="projectiles/Serpent_Staff";
  U72Shapes[641]="scenery/key";
  U72Shapes[642]="scenery/book";
  U72Shapes[643]="trees/tree";
  U72Shapes[644]="scenery/gold_coin";
  U72Shapes[645]="scenery/gold_nugget";
  U72Shapes[646]="scenery/gold_bar";
  U72Shapes[647]="units/female_pikeman";
  U72Shapes[648]="scenery/sleeping_powder";
  U72Shapes[649]="scenery/artifact";
  U72Shapes[650]="scenery/sextant";
  U72Shapes[651]="scenery/spinning_wheel";
  U72Shapes[652]="units/woman";
  U72Shapes[653]="scenery/bale_of_wool";
  U72Shapes[654]="scenery/spindle_of_thread";
  U72Shapes[655]="scenery/Planet";
  U72Shapes[656]="scenery/wine_press";
  U72Shapes[657]="walls/curtain";
  U72Shapes[658]="units/automaton";
  U72Shapes[659]="projectiles/mace";
  U72Shapes[660]="units/skinless_woman";
  U72Shapes[661]="units/mongbat";
  U72Shapes[662]="projectiles/fishing_rod";
  U72Shapes[663]="scenery/magic_shield";
  U72Shapes[664]="scenery/stove";
  U72Shapes[665]="units/dark_female_monk";
  U72Shapes[666]="scenery/magic_armor";
  U72Shapes[667]="scenery/cobwebs";
  U72Shapes[668]="scenery/metal";
  U72Shapes[669]="units/gwenno";
  U72Shapes[670]="trees/maple_tree";
  U72Shapes[671]="trees/mushrooms";
  U72Shapes[672]="trees/shrubbery";
  U72Shapes[673]="trees/strange_plant";
  U72Shapes[674]="trees/silverleaf_tree";
  U72Shapes[675]="scenery/desk_item";
  U72Shapes[676]="projectiles/unnamed";
  U72Shapes[677]="scenery/stockings";
  U72Shapes[678]="walls/curtain";
  U72Shapes[679]="scenery/drawers";
  U72Shapes[680]="scenery/deck";
  U72Shapes[681]="scenery/jar";
  U72Shapes[682]="scenery/glass_item";
  U72Shapes[683]="trees/tree";
  U72Shapes[684]="scenery/trellis";
  U72Shapes[685]="scenery/grapevine";
  U72Shapes[686]="scenery/magic_leggings";
  U72Shapes[687]="scenery/pillar";
  U72Shapes[688]="scenery/serpent_carving";
  U72Shapes[689]="scenery/harpsichord";
  U72Shapes[690]="scenery/xylophone";
  U72Shapes[691]="units/goblin";
  U72Shapes[692]="scenery/lute";
  U72Shapes[693]="scenery/whistle";
  U72Shapes[694]="walls/cave_door";
  U72Shapes[695]="scenery/grandfather_clock";
  U72Shapes[696]="scenery/bed";
  U72Shapes[697]="scenery/podium";
  U72Shapes[698]="scenery/shears";
  U72Shapes[699]="tiles/surf";
  U72Shapes[700]="scenery/deck";
  U72Shapes[701]="projectiles/torch_lit";
  U72Shapes[702]="units/phoenix";
  U72Shapes[703]="units/ice_corpser";
  U72Shapes[704]="scenery/keg";
  U72Shapes[705]="scenery/book";
  U72Shapes[706]="units/scorpion";
  U72Shapes[707]="scenery/scroll";
  U72Shapes[708]="scenery/stocks";
  U72Shapes[709]="scenery/plaque";
  U72Shapes[710]="projectiles/ophidian_sword";
  U72Shapes[711]="projectiles/infinity_bow";
  U72Shapes[712]="scenery/stove";
  U72Shapes[713]="scenery/post";
  U72Shapes[714]="scenery/basin";
  U72Shapes[715]="scenery/magic_scroll";
  U72Shapes[716]="units/bird";
  U72Shapes[717]="scenery/plate";
  U72Shapes[718]="scenery/pedestal";
  U72Shapes[719]="scenery/water_trough";
  U72Shapes[720]="units/guard";
  U72Shapes[721]="units/avatar";
  U72Shapes[722]="projectiles/arrow";
  U72Shapes[723]="projectiles/bolt";
  U72Shapes[724]="scenery/pedestal";
  U72Shapes[725]="units/goblin";
  U72Shapes[726]="projectiles/pulsating_object";
  U72Shapes[727]="scenery/dream_crystal";
  U72Shapes[728]="walls/force_wall";
  U72Shapes[729]="scenery/shield_of_monitor";
  U72Shapes[730]="scenery/dead_baby";
  U72Shapes[731]="projectiles/unnamed";
  U72Shapes[732]="walls/window";
  U72Shapes[733]="tiles/carpet";
  U72Shapes[734]="units/unnamed";
  U72Shapes[735]="scenery/archery_target";
  U72Shapes[736]="tiles/surf";
  U72Shapes[737]="tiles/surf";
  U72Shapes[738]="walls/wall";
  U72Shapes[739]="scenery/firepit";
  U72Shapes[740]="scenery/wine_press";
  U72Shapes[741]="scenery/water_trough";
  U72Shapes[742]="units/wench";
  U72Shapes[743]="scenery/statue";
  U72Shapes[744]="units/goblin";
  U72Shapes[745]="scenery/harp";
  U72Shapes[746]="scenery/pillar";
  U72Shapes[747]="units/automaton";
  U72Shapes[748]="walls/wall";
  U72Shapes[749]="scenery/alchemist_apparatus";
  U72Shapes[750]="tiles/carpet";
  U72Shapes[751]="tiles/surf";
  U72Shapes[752]="scenery/music_box";
  U72Shapes[753]="units/stone_harpy";
  U72Shapes[754]="units/leopard";
  U72Shapes[755]="scenery/ice_block";
  U72Shapes[756]="scenery/caltrops";
  U72Shapes[757]="walls/wall";
  U72Shapes[758]="tiles/carpet";
  U72Shapes[759]="scenery/bell";
  U72Shapes[760]="scenery/gem";
  U72Shapes[761]="scenery/spellbook";
  U72Shapes[762]="units/body";
  U72Shapes[763]="units/ice_worm";
  U72Shapes[764]="walls/wall";
  U72Shapes[765]="walls/wall";
  U72Shapes[766]="units/woman";
  U72Shapes[767]="walls/wall";
  U72Shapes[768]="projectiles/energy_field";
  U72Shapes[769]="walls/wall";
  U72Shapes[770]="walls/wall";
  U72Shapes[771]="projectiles/rudyoms_wand";
  U72Shapes[772]="units/parrot";
  U72Shapes[773]="walls/wall";
  U72Shapes[774]="walls/wall";
  U72Shapes[775]="scenery/ship_rails";
  U72Shapes[776]="scenery/podium";
  U72Shapes[777]="scenery/Pillar";
  U72Shapes[778]="units/body";
  U72Shapes[779]="tiles/magical_wave";
  U72Shapes[780]="tiles/bubbles";
  U72Shapes[781]="walls/gang_plank";
  U72Shapes[782]="walls/wall";
  U72Shapes[783]="trees/tree";
  U72Shapes[784]="scenery/web_of_fate";
  U72Shapes[785]="scenery/pillar";
  U72Shapes[786]="scenery/table";
  U72Shapes[787]="scenery/lever";
  U72Shapes[788]="scenery/switch";
  U72Shapes[789]="scenery/assimilation_machine";
  U72Shapes[790]="scenery/engine";
  U72Shapes[791]="scenery/ship";
  U72Shapes[792]="projectiles/magicians_wand";
  U72Shapes[793]="units/lord_british";
  U72Shapes[794]="units/severed_limb";
  U72Shapes[795]="scenery/mantle";
  U72Shapes[796]="scenery/campfire";
  U72Shapes[797]="scenery/scroll";
  U72Shapes[798]="units/baby_ice_dragon";
  U72Shapes[799]="units/body_part";
  U72Shapes[800]="scenery/chest";
  U72Shapes[801]="scenery/backpack";
  U72Shapes[802]="scenery/bag";
  U72Shapes[803]="scenery/basket";
  U72Shapes[804]="scenery/crate";
  U72Shapes[805]="units/fighter";
  U72Shapes[806]="projectiles/black_sword";
  U72Shapes[807]="projectiles/lightning";
  U72Shapes[808]="tiles/surf";
  U72Shapes[809]="units/mage";
  U72Shapes[810]="scenery/bucket";
  U72Shapes[811]="units/rabbit";
  U72Shapes[812]="scenery/iron_maiden";
  U72Shapes[813]="scenery/rack";
  U72Shapes[814]="units/mage";
  U72Shapes[815]="units/monster";
  U72Shapes[816]="units/fighter";
  U72Shapes[817]="units/man";
  U72Shapes[818]="units/counselor";
  U72Shapes[819]="scenery/barrel";
  U72Shapes[820]="scenery/plaque";
  U72Shapes[821]="walls/painting";
  U72Shapes[822]="scenery/diaper";
  U72Shapes[823]="scenery/artists_equipment";
  U72Shapes[824]="scenery/pitcher";
  U72Shapes[825]="scenery/fire";
  U72Shapes[826]="walls/wall";
  U72Shapes[827]="scenery/bandage";
  U72Shapes[828]="walls/wall";
  U72Shapes[829]="tiles/floor";
  U72Shapes[830]="units/man";
  U72Shapes[831]="scenery/baking_hearth";
  U72Shapes[832]="units/frost_serpent";
  U72Shapes[833]="tiles/glass_wall";
  U72Shapes[834]="tiles/surf";
  U72Shapes[835]="scenery/magic_gauntlets";
  U72Shapes[836]="scenery/antique_armor";
  U72Shapes[837]="scenery/easel";
  U72Shapes[838]="scenery/bird_cage";
  U72Shapes[839]="scenery/hourglass";
  U72Shapes[840]="tiles/chasm";
  U72Shapes[841]="scenery/glass_wall";
  U72Shapes[842]="scenery/reagent";
  U72Shapes[843]="tiles/chasm";
  U72Shapes[844]="tiles/chasm";
  U72Shapes[845]="walls/Wall";
  U72Shapes[846]="units/naga";
  U72Shapes[847]="scenery/table";
  U72Shapes[848]="scenery/mirror";
  U72Shapes[849]="walls/tapestry";
  U72Shapes[850]="tiles/carpet";
  U72Shapes[851]="scenery/cloth";
  U72Shapes[852]="walls/crenellations";
  U72Shapes[853]="units/ghost";
  U72Shapes[854]="units/statue";
  U72Shapes[855]="units/captain";
  U72Shapes[856]="projectiles/fire_bolt";
  U72Shapes[857]="projectiles/dragon_breath";
  U72Shapes[858]="scenery/bookshelf";
  U72Shapes[859]="projectiles/casting_frames";
  U72Shapes[860]="scenery/fencing_dummy";
  U72Shapes[861]="units/fire_elemental";
  U72Shapes[862]="units/gwani";
  U72Shapes[863]="scenery/kitchen_item";
  U72Shapes[864]="scenery/moongate";
  U72Shapes[865]="units/spider";
  U72Shapes[866]="scenery/thumbscrews";
  U72Shapes[867]="units/gwani_child";
  U72Shapes[868]="walls/wall_mount";
  U72Shapes[869]="walls/wall";
  U72Shapes[870]="walls/drawbridge";
  U72Shapes[871]="walls/wall";
  U72Shapes[872]="scenery/stove";
  U72Shapes[873]="scenery/chair";
  U72Shapes[874]="units/hound";
  U72Shapes[875]="tiles/surf";
  U72Shapes[876]="walls/sliding_door";
  U72Shapes[877]="units/ice_dragon";
  U72Shapes[878]="walls/crenellations";
  U72Shapes[879]="walls/wall";
  U72Shapes[880]="units/mad_iolo";
  U72Shapes[881]="scenery/shattered_statue";
  U72Shapes[882]="units/winged_viper";
  U72Shapes[883]="units/gargoyle_wingless";
  U72Shapes[884]="walls/melted_door";
  U72Shapes[885]="units/nightmare";
  U72Shapes[886]="scenery/unnamed";
  U72Shapes[887]="scenery/ring";
  U72Shapes[888]="units/mage";
  U72Shapes[889]="scenery/lamp_post";
  U72Shapes[890]="scenery/table";
  U72Shapes[891]="units/avatar";
  U72Shapes[892]="units/body";
  U72Shapes[893]="scenery/avatar";
  U72Shapes[894]="scenery/monolith";
  U72Shapes[895]="projectiles/fire_field";
  U72Shapes[896]="walls/crenellations";
  U72Shapes[897]="scenery/seat";
  U72Shapes[898]="scenery/altar";
  U72Shapes[899]="scenery/wall";
  U72Shapes[900]="projectiles/poison_field";
  U72Shapes[901]="walls/crenellations";
  U72Shapes[902]="projectiles/sleep_field";
  U72Shapes[903]="walls/crenellations";
  U72Shapes[904]="walls/wall";
  U72Shapes[905]="walls/cavern";
  U72Shapes[906]="units/anti_dupre";
  U72Shapes[907]="tiles/surf";
  U72Shapes[908]="roofs/tile_roof";
  U72Shapes[909]="walls/wall";
  U72Shapes[910]="walls/wall";
  U72Shapes[911]="tiles/surf";
  U72Shapes[912]="scenery/blood";
  U72Shapes[913]="scenery/pillar";
  U72Shapes[914]="scenery/urn";
  U72Shapes[915]="units/tentacle";
  U72Shapes[916]="units/mageling";
  U72Shapes[917]="units/trapper";
  U72Shapes[918]="tiles/surf";
  U72Shapes[919]="scenery/bookshelf";
  U72Shapes[920]="scenery/pillar";
  U72Shapes[921]="units/avatar";
  U72Shapes[922]="trees/small_bush";
  U72Shapes[923]="scenery/piece_of_wood";
  U72Shapes[924]="walls/cavern";
  U72Shapes[925]="walls/cavern";
  U72Shapes[926]="scenery/serpent_sceptre";
  U72Shapes[927]="scenery/serpent_slot";
  U72Shapes[928]="walls/cavern";
  U72Shapes[929]="scenery/philanderers_friend";
  U72Shapes[930]="units/avatar";
  U72Shapes[931]="tiles/mountain";
  U72Shapes[932]="trees/tree";
  U72Shapes[933]="units/avatar";
  U72Shapes[934]="scenery/magic_telescope";
  U72Shapes[935]="walls/sliding_door";
  U72Shapes[936]="walls/sliding_door";
  U72Shapes[937]="scenery/jewelry";
  U72Shapes[938]="units/avatar";
  U72Shapes[939]="walls/wall";
  U72Shapes[940]="walls/wall";
  U72Shapes[941]="tiles/unnamed";
  U72Shapes[942]="projectiles/hammer_of_dedication";
  U72Shapes[943]="walls/wall";
  U72Shapes[944]="scenery/pot";
  U72Shapes[945]="units/unnamed";
  U72Shapes[946]="units/unnamed";
  U72Shapes[947]="units/unnamed";
  U72Shapes[948]="scenery/filari";
  U72Shapes[949]="scenery/winch";
  U72Shapes[950]="scenery/winch";
  U72Shapes[951]="scenery/monetari";
  U72Shapes[952]="scenery/guilder";
  U72Shapes[953]="walls/wall";
  U72Shapes[954]="roofs/glass_roof";
  U72Shapes[955]="scenery/amulet";
  U72Shapes[956]="roofs/wood_roof";
  U72Shapes[957]="units/catman";
  U72Shapes[958]="scenery/unnamed";
  U72Shapes[959]="tiles/mountain";
  U72Shapes[960]="trees/mushroom";
  U72Shapes[961]="scenery/barge";
  U72Shapes[962]="roofs/slate_roof";
  U72Shapes[963]="roofs/slate_roof";
  U72Shapes[964]="scenery/table";
  U72Shapes[965]="scenery/glowing_sphere";
  U72Shapes[966]="roofs/tile_roof";
  U72Shapes[967]="walls/broken_wall";
  U72Shapes[968]="units/ice_creature";
  U72Shapes[969]="tiles/mountain";
  U72Shapes[970]="units/sheep";
  U72Shapes[971]="scenery/table";
  U72Shapes[972]="walls/wall";
  U72Shapes[973]="walls/stairs";
  U72Shapes[974]="walls/stairs";
  U72Shapes[975]="units/unnamed";
  U72Shapes[976]="scenery/dartboard";
  U72Shapes[977]="scenery/fur_pelt";
  U72Shapes[978]="units/dragon";
  U72Shapes[979]="units/spiderman";
  U72Shapes[980]="walls/wall";
  U72Shapes[981]="units/liche";
  U72Shapes[982]="roofs/chimney";
  U72Shapes[983]="tiles/mountain";
  U72Shapes[984]="scenery/piece_of_wood";
  U72Shapes[985]="projectiles/waves";
  U72Shapes[986]="scenery/stove_top";
  U72Shapes[987]="scenery/cradle";
  U72Shapes[988]="scenery/magic_compass";
  U72Shapes[989]="units/avatar";
  U72Shapes[990]="projectiles/hammer";
  U72Shapes[991]="scenery/anvil";
  U72Shapes[992]="scenery/burning_urn";
  U72Shapes[993]="trees/mushroom";
  U72Shapes[994]="scenery/tongs";
  U72Shapes[995]="scenery/cauldron";
  U72Shapes[996]="scenery/unnamed";
  U72Shapes[997]="scenery/light_source_spent";
  U72Shapes[998]="units/statue";
  U72Shapes[999]="scenery/plant";
  U72Shapes[1000]="scenery/table";
  U72Shapes[1001]="scenery/unnamed";
  U72Shapes[1002]="scenery/table";
  U72Shapes[1003]="scenery/table";
  U72Shapes[1004]="scenery/helm";
  U72Shapes[1005]="scenery/bellows";
  U72Shapes[1006]="walls/broken_wall";
  U72Shapes[1007]="scenery/crematorium_machine";
  U72Shapes[1008]="projectiles/waves";
  U72Shapes[1009]="projectiles/waves";
  U72Shapes[1010]="scenery/crematorium_machine";
  U72Shapes[1011]="scenery/bed";
  U72Shapes[1012]="tiles/surf";
  U72Shapes[1013]="scenery/unnamed";
  U72Shapes[1014]="tiles/floor";
  U72Shapes[1015]="units/unnamed";
  U72Shapes[1016]="scenery/floor";
  U72Shapes[1017]="scenery/ship";
  U72Shapes[1018]="scenery/table";
  U72Shapes[1019]="scenery/crematorium_machine";
  U72Shapes[1020]="tiles/surf";
  U72Shapes[1021]="trees/berry_bush";
  U72Shapes[1022]="tiles/surf";
  U72Shapes[1023]="trees/unnamed";
  U72Shapes[1024]="units/avatar_male";
  U72Shapes[1025]="units/avatar_female";
  U72Shapes[1026]="units/avatar_male";
  U72Shapes[1027]="units/avatar_female";
  U72Shapes[1028]="units/avatar_male";
  U72Shapes[1029]="units/avatar_female";
  U72Shapes[1030]="units/naked_male";
  U72Shapes[1031]="units/naked_female";
  U72Shapes[1032]="units/naked_male";
  U72Shapes[1033]="units/naked_female";
  U72Shapes[1034]="units/naked_male";
  U72Shapes[1035]="units/naked_female";
}

static char *u8shapes[] = {
  /*0000*/ "Dummy",
  /*0001*/ "Characters/Avatar",
  /*0002*/ "Dummy",
  /*0003*/ "Floor/001",
  /*0004*/ "Floor/002",
  /*0005*/ "Characters/AvatarGibs1",
  /*0006*/ "Floor/003",
  /*0007*/ "Wall/001",
  /*0008*/ "Wall/002",
  /*0009*/ "Floor/04",
  /*0010*/ "Wall/003",
  /*0011*/ "Floor/005",
  /*0012*/ "Wall/004",
  /*0013*/ "Wall/005",
  /*0014*/ "Wall/007",
  /*0015*/ "Wall/008",
  /*0016*/ "Wall/009",
  /*0017*/ "Wall/010",
  /*0018*/ "Wall/011",
  /*0019*/ "Wall/012",
  /*0020*/ "Wall/013",
  /*0021*/ "Wall/014",
  /*0022*/ "Wall/015",
  /*0023*/ "Wall/016",
  /*0024*/ "Wall/017",
  /*0025*/ "Wall/018",
  /*0026*/ "Wall/019",
  /*0027*/ "Wall/020",
  /*0028*/ "Wall/021",
  /*0029*/ "Wall/022",
  /*0030*/ "Wall/023",
  /*0031*/ "Wall/024",
  /*0032*/ "Wall/025",
  /*0033*/ "Wall/026",
  /*0034*/ "Floor/006",
  /*0035*/ "Floor/007",
  /*0036*/ "Floor/008",
  /*0037*/ "Floor/009",
  /*0038*/ "Wall/027",
  /*0039*/ "Wall/028",
  /*0040*/ "Wall/029",
  /*0041*/ "Wall/030",
  /*0042*/ "Wall/031",
  /*0043*/ "Wall/032",
  /*0044*/ "Objects/Statue01",
  /*0045*/ "Floor/010",
  /*0046*/ "Wall/033",
  /*0047*/ "Wall/034",
  /*0048*/ "Floor/010",
  /*0049*/ "Floor/011",
  /*0050*/ "Floor/012",
  /*0051*/ "Floor/013",
  /*0052*/ "Wall/035",
  /*0053*/ "Floor/014",
  /*0054*/ "Floor/015",
  /*0055*/ "Floor/016",
  /*0056*/ "Objects/Nature01",
  /*0057*/ "Objects/Nature02",
  /*0058*/ "Wall/036",
  /*0059*/ "Effects/TrapElectricGateway1",
  /*0060*/ "Objects/Nature03",
  /*0061*/ "Objects/0061",
  /*0062*/ "Wall/037",
  /*0063*/ "Objects/01",
  /*0064*/ "PaperDoll/MagicArmor1",
  /*0065*/ "Objects/Nature04",
  /*0066*/ "Objects/03",
  /*0067*/ "Objects/04",
  /*0068*/ "Objects/Door01",
  /*0069*/ "Objects/Door02",
  /*0070*/ "Floor/017",
  /*0071*/ "Floor/018",
  /*0072*/ "Objects/05",
  /*0073*/ "GUI/01",
  /*0074*/ "Objects/Nature05",
  /*0075*/ "Objects/Nature06",
  /*0076*/ "Units/Kith",
  /*0077*/ "Floor/019",
  /*0078*/ "Objects/06",
  /*0079*/ "Items/Keyring",
  /*0080*/ "Characters/Hydros1",
  /*0081*/ "Units/Lurker",
  /*0082*/ "Items/Keys",
  /*0083*/ "Units/Troll",
  /*0084*/ "Floor/020",
  /*0085*/ "Wall/038",
  /*0086*/ "Wall/039",
  /*0087*/ "Wall/040",
  /*0088*/ "Wall/041",
  /*0089*/ "Wall/042",
  /*0090*/ "Wall/043",
  /*0091*/ "Wall/044",
  /*0092*/ "Floor/021",
  /*0093*/ "Floor/022",
  /*0094*/ "Floor/023",
  /*0095*/ "Floor/024",
  /*0096*/ "Units/Daemon",
  /*0097*/ "Characters/Toran",
  /*0098*/ "Floor/025",
  /*0099*/ "Wall/045",
  /*0100*/ "Wall/046",
  /*0101*/ "Wall/047",
  /*0102*/ "Wall/048",
  /*0103*/ "Wall/049",
  /*0104*/ "Wall/050",
  /*0105*/ "Wall/051",
  /*0106*/ "Wall/052",
  /*0107*/ "Wall/053",
  /*0108*/ "Wall/054",
  /*0109*/ "Characters/Pyros1",
  /*0110*/ "Characters/Pyros2",
  /*0111*/ "Characters/Pyros3",
  /*0112*/ "Wall/055",
  /*0113*/ "Floor/026",
  /*0114*/ "Objects/Door03",
  /*0115*/ "Objects/07",
  /*0116*/ "Objects/08",
  /*0117*/ "Objects/09",
  /*0118*/ "Floor/027",
  /*0119*/ "Units/Skeleton",
  /*0120*/ "Units/Torax",
  /*0121*/ "Objects/10",
  /*0122*/ "Floor/028",
  /*0123*/ "Effects/02",
  /*0124*/ "Characters/Pyros4",
  /*0125*/ "Floor/029",
  /*0126*/ "Floor/030",
  /*0127*/ "Floor/031",
  /*0128*/ "Floor/032",
  /*0129*/ "Objects/Statue02",
  /*0130*/ "Wall/056",
  /*0131*/ "Wall/057",
  /*0132*/ "Wall/058",
  /*0133*/ "Wall/059",
  /*0134*/ "Wall/060",
  /*0135*/ "Objects/Door04",
  /*0136*/ "Objects/Door05",
  /*0137*/ "Characters/Devon",
  /*0138*/ "Wall/061",
  /*0139*/ "Wall/062",
  /*0140*/ "Wall/063",
  /*0141*/ "Objects/11",
  /*0142*/ "Units/Ghoul",
  /*0143*/ "Items/Obsidian",
  /*0144*/ "Characters/Shaana",
  /*0145*/ "Wall/064",
  /*0146*/ "Wall/065",
  /*0147*/ "Wall/066",
  /*0148*/ "Wall/067",
  /*0149*/ "Wall/068",
  /*0150*/ "Wall/069",
  /*0151*/ "Wall/070",
  /*0152*/ "Wall/071",
  /*0153*/ "Wall/072",
  /*0154*/ "Wall/073",
  /*0155*/ "Wall/074",
  /*0156*/ "Wall/075",
  /*0157*/ "Wall/076",
  /*0158*/ "Wall/077",
  /*0159*/ "Items/Scrolls1",
  /*0160*/ "Wall/078",
  /*0161*/ "Wall/079",
  /*0162*/ "Floor/033",
  /*0163*/ "Floor/034",
  /*0164*/ "Floor/035",
  /*0165*/ "Floor/036",
  /*0166*/ "Effects/03",
  /*0167*/ "Wall/080",
  /*0168*/ "Wall/081",
  /*0169*/ "Wall/082",
  /*0170*/ "Wall/083",
  /*0171*/ "Wall/084",
  /*0172*/ "Wall/085",
  /*0173*/ "Wall/086",
  /*0174*/ "GUI/Digits",
  /*0175*/ "Items/Beds1",
  /*0176*/ "Items/Pots",
  /*0177*/ "Items/Barrel",
  /*0178*/ "Items/Mugs",
  /*0179*/ "Items/0179",
  /*0180*/ "Items/Bottles1",
  /*0181*/ "Items/Dishes01",
  /*0182*/ "Items/Goblets",
  /*0183*/ "Items/Jars01",
  /*0184*/ "Items/Forks",
  /*0185*/ "Items/Baskets",
  /*0186*/ "Objects/12",
  /*0187*/ "Characters/Malchir",
  /*0188*/ "Characters/Stellos",
  /*0189*/ "Characters/Mordrea",
  /*0190*/ "Characters/Mythran",
  /*0191*/ "Characters/Vardion",
  /*0192*/ "Characters/Beren",
  /*0193*/ "Objects/13",
  /*0194*/ "Objects/Door06",
  /*0195*/ "Objects/Door07",
  /*0196*/ "Items/Beds2",
  /*0197*/ "Objects/13",
  /*0198*/ "Floor/037",
  /*0199*/ "Objects/Nature07",
  /*0200*/ "Characters/Sorcerer",
  /*0201*/ "Characters/HydrosCircle",
  /*0202*/ "Effects/04",
  /*0203*/ "Objects/14",
  /*0204*/ "Objects/15",
  /*0205*/ "Objects/16",
  /*0206*/ "Objects/17",
  /*0207*/ "Wall/087",
  /*0208*/ "Objects/18",
  /*0209*/ "Units/ToraxDead",
  /*0210*/ "Floor/038",
  /*0211*/ "Floor/039",
  /*0212*/ "Floor/040",
  /*0213*/ "Objects/19",
  /*0214*/ "Units/Golem",
  /*0215*/ "Items/Gems1",
  /*0216*/ "Effects/05",
  /*0217*/ "Objects/Nature08",
  /*0218*/ "Objects/20",
  /*0219*/ "Objects/21",
  /*0220*/ "Floor/041",
  /*0221*/ "Objects/22",
  /*0222*/ "Objects/23",
  /*0223*/ "Objects/24",
  /*0224*/ "Objects/25",
  /*0225*/ "Objects/26",
  /*0226*/ "Objects/27",
  /*0227*/ "Objects/28",
  /*0228*/ "Items/Flask",
  /*0229*/ "Items/Jars02",
  /*0230*/ "Effects/05",
  /*0231*/ "Objects/29",
  /*0232*/ "Items/NecromanticKeys",
  /*0233*/ "Objects/30",
  /*0234*/ "Items/Paintings01",
  /*0235*/ "Items/Bones01",
  /*0236*/ "Items/Paintings02",
  /*0237*/ "Items/Bones02",
  /*0238*/ "Items/Paintings03",
  /*0239*/ "Items/Paintings04",
  /*0240*/ "Floor/042",
  /*0241*/ "Weapons/HammerOverlay",
  /*0242*/ "Floor/043",
  /*0243*/ "Weapons/SwordOverlay",
  /*0244*/ "Weapons/AxeOverlay",
  /*0245*/ "Weapons/KnifeOverlay",
  /*0246*/ "Wall/088",
  /*0247*/ "Wall/089",
  /*0248*/ "Wall/090",
  /*0249*/ "Wall/091",
  /*0250*/ "Wall/092",
  /*0251*/ "Wall/093",
  /*0252*/ "Wall/094",
  /*0253*/ "Wall/095",
  /*0254*/ "Wall/096",
  /*0255*/ "Wall/097",
  /*0256*/ "Effects/06",
  /*0257*/ "Wall/098",
  /*0258*/ "Characters/Lothian",
  /*0259*/ "Objects/31",
  /*0260*/ "GUI/03",
  /*0261*/ "Effects/07",
  /*0262*/ "Floor/044",
  /*0263*/ "Objects/32",
  /*0264*/ "Effects/07",
  /*0265*/ "Effects/08",
  /*0266*/ "Characters/Gwillim",
  /*0267*/ "Objects/Campfire",
  /*0268*/ "Wall/099",
  /*0269*/ "Characters/Theurgist",
  /*0270*/ "Floor/045",
  /*0271*/ "Floor/046",
  /*0272*/ "Wall/100",
  /*0273*/ "Objects/33",
  /*0274*/ "Objects/34",
  /*0275*/ "Objects/Statue03",
  /*0276*/ "Objects/35",
  /*0277*/ "Objects/36",
  /*0278*/ "Objects/37",
  /*0279*/ "Objects/38",
  /*0280*/ "Objects/Oven",
  /*0281*/ "Objects/39",
  /*0282*/ "Characters/Girld",
  /*0283*/ "Objects/40",
  /*0284*/ "Items/Dishes02",
  /*0285*/ "Wall/101",
  /*0286*/ "Floor/047",
  /*0287*/ "Wall/102",
  /*0288*/ "Items/Books01",
  /*0289*/ "Objects/40",
  /*0290*/ "Wall/103",
  /*0291*/ "Wall/104",
  /*0292*/ "Wall/105",
  /*0293*/ "Wall/106",
  /*0294*/ "Wall/107",
  /*0295*/ "Wall/108",
  /*0296*/ "Wall/109",
  /*0297*/ "Wall/110",
  /*0298*/ "Wall/111",
  /*0299*/ "Floor/048",
  /*0300*/ "Floor/049",
  /*0301*/ "Floor/050",
  /*0302*/ "Floor/051",
  /*0303*/ "Floor/052",
  /*0304*/ "Floor/053",
  /*0305*/ "Floor/054",
  /*0306*/ "Objects/Wardrobe1",
  /*0307*/ "Objects/Wardrobe2",
  /*0308*/ "Items/0308",
  /*0309*/ "Objects/Altar",
  /*0310*/ "Objects/41",
  /*0311*/ "Floor/055",
  /*0312*/ "Floor/056",
  /*0313*/ "Wall/112",
  /*0314*/ "Objects/42",
  /*0315*/ "Characters/Bane",
  /*0316*/ "Characters/Gorgond",
  /*0317*/ "Objects/43",
  /*0318*/ "Objects/44",
  /*0319*/ "Objects/45",
  /*0320*/ "Objects/46",
  /*0321*/ "Objects/Nature09",
  /*0322*/ "Objects/Nature10",
  /*0323*/ "Objects/47",
  /*0324*/ "Objects/48",
  /*0325*/ "Objects/49",
  /*0326*/ "Objects/50",
  /*0327*/ "Objects/51",
  /*0328*/ "Items/Banners01",
  /*0329*/ "Items/Banners02",
  /*0330*/ "Objects/52",
  /*0331*/ "Items/Bones03",
  /*0332*/ "Items/Bones04",
  /*0333*/ "Objects/53",
  /*0334*/ "Floor/057",
  /*0335*/ "Floor/058",
  /*0336*/ "Effects/09",
  /*0337*/ "Effects/10",
  /*0338*/ "Effects/11",
  /*0339*/ "Characters/Vividos",
  /*0340*/ "Objects/Door08",
  /*0341*/ "Objects/Door09",
  /*0342*/ "Objects/Door10",
  /*0343*/ "Floor/059",
  /*0344*/ "Wall/113",
  /*0345*/ "Wall/114",
  /*0346*/ "Items/ElectronicArtsLogo",
  /*0347*/ "Floor/060",
  /*0348*/ "Objects/54",
  /*0349*/ "Floor/061",
  /*0350*/ "Objects/55",
  /*0351*/ "Floor/Stars",
  /*0352*/ "Floor/062",
  /*0353*/ "Floor/063",
  /*0354*/ "Floor/064",
  /*0355*/ "Floor/065",
  /*0356*/ "Floor/066",
  /*0357*/ "Units/Changeling",
  /*0358*/ "Items/Bones05",
  /*0359*/ "Items/Bones06",
  /*0360*/ "Characters/AvatarGibs2",
  /*0361*/ "Characters/Stratos1",
  /*0362*/ "Characters/Stratos2",
  /*0363*/ "Wall/115",
  /*0364*/ "Wall/116",
  /*0365*/ "Wall/117",
  /*0366*/ "Characters/AvatarGibs3",
  /*0367*/ "Floor/067",
  /*0368*/ "Wall/117",
  /*0369*/ "Wall/118",
  /*0370*/ "Wall/119",
  /*0371*/ "Wall/120",
  /*0372*/ "Characters/AvatarGibs4",
  /*0373*/ "Characters/AvatarGibs5",
  /*0374*/ "Characters/AvatarGibs6",
  /*0375*/ "Items/Abacus",
  /*0376*/ "Dummy",
  /*0377*/ "Units/AirServant",
  /*0378*/ "Items/Books02",
  /*0379*/ "Dummy",
  /*0380*/ "Dummy",
  /*0381*/ "Characters/Pyros5",
  /*0382*/ "Characters/Pyros6",
  /*0383*/ "Effects/12",
  /*0384*/ "Characters/Pyros7",
  /*0385*/ "Characters/Stratos3",
  /*0386*/ "Floor/068",
  /*0387*/ "Characters/Cyrrus",
  /*0388*/ "Characters/Xavier",
  /*0389*/ "Characters/Torwin",
  /*0390*/ "Characters/AvatarGibs7",
  /*0391*/ "Objects/Nature11",
  /*0392*/ "Wall/121",
  /*0393*/ "Floor/069",
  /*0394*/ "Wall/122",
  /*0395*/ "Items/NecromancyReagents",
  /*0396*/ "Items/TheurgySpells",
  /*0397*/ "GUI/Cursors2",
  /*0398*/ "Items/Reagents",
  /*0399*/ "Items/Reagents2",
  /*0400*/ "Effects/13",
  /*0401*/ "Characters/Darion",
  /*0402*/ "Characters/Jenna",
  /*0403*/ "Characters/Orlok",
  /*0404*/ "Characters/Salkind",
  /*0405*/ "Characters/Tarna",
  /*0406*/ "Effects/14",
  /*0407*/ "Dummy",
  /*0408*/ "Effects/15",
  /*0409*/ "Floor/Lava1",
  /*0410*/ "Floor/Lava2",
  /*0411*/ "Units/Ghost",
  /*0412*/ "Objects/Roof1",
  /*0413*/ "Units/FireSkull",
  /*0414*/ "Effects/16",
  /*0415*/ "Effects/17",
  /*0416*/ "Effects/18",
  /*0417*/ "PaperDoll/Mace",
  /*0418*/ "PaperDoll/Knife",
  /*0419*/ "PaperDoll/Axe",
  /*0420*/ "PaperDoll/Sword",
  /*0421*/ "Effects/19",
  /*0422*/ "Effects/20",
  /*0423*/ "Effects/21",
  /*0424*/ "Effects/22",
  /*0425*/ "Floor/070",
  /*0426*/ "Objects/Nature12",
  /*0427*/ "Objects/Nature13",
  /*0428*/ "Dummy",
  /*0429*/ "Characters/LothianSacrifice",
  /*0430*/ "Characters/Bentic",
  /*0431*/ "Characters/Korick",
  /*0432*/ "Characters/Kilandra",
  /*0433*/ "Characters/Lithos",
  /*0434*/ "Objects/Nature14",
  /*0435*/ "Floor/Waterfall01",
  /*0436*/ "Floor/Waterfall02",
  /*0437*/ "Floor/Waterfall03",
  /*0438*/ "Floor/Waterfall04",
  /*0439*/ "Floor/Waterfall05",
  /*0440*/ "Items/0440",
  /*0441*/ "Floor/Cursor01",
  /*0442*/ "Items/FireBlackrock",
  /*0443*/ "Objects/Statue04",
  /*0444*/ "Items/Beds3",
  /*0445*/ "Items/Beds5",
  /*0446*/ "Items/Beds6",
  /*0447*/ "Items/Beds7",
  /*0448*/ "Items/Beds4",
  /*0449*/ "Objects/56",
  /*0450*/ "Objects/57",
  /*0451*/ "Floor/Cursor02",
  /*0452*/ "Items/Candle1",
  /*0453*/ "Items/Candle2",
  /*0454*/ "Items/Candle3",
  /*0455*/ "Wall/123",
  /*0456*/ "Wall/124",
  /*0457*/ "Wall/125",
  /*0458*/ "Items/0458",
  /*0459*/ "Wall/126",
  /*0460*/ "Wall/127",
  /*0461*/ "Wall/128",
  /*0462*/ "Wall/129",
  /*0463*/ "Wall/130",
  /*0464*/ "Wall/131",
  /*0465*/ "Objects/Nature15",
  /*0466*/ "Objects/Nature16",
  /*0467*/ "Effects/Cracks1",
  /*0468*/ "Effects/Cracks2",
  /*0469*/ "Floor/071",
  /*0470*/ "Effects/Cracks3",
  /*0471*/ "Items/Garbage4",
  /*0472*/ "Floor/072",
  /*0473*/ "Items/Garbage5",
  /*0474*/ "Floor/Cursor03",
  /*0475*/ "Effects/WaterDrop1",
  /*0476*/ "Effects/WaterDrop2",
  /*0477*/ "Effects/WaterDrop3",
  /*0478*/ "Floor/Lava3",
  /*0479*/ "Floor/Lava4",
  /*0480*/ "Effects/LavaDrop1",
  /*0481*/ "Effects/Wind1",
  /*0482*/ "Effects/WaterDrop4",
  /*0483*/ "Effects/LavaDrop2",
  /*0484*/ "Effects/Blood1",
  /*0485*/ "Characters/Corinth",
  /*0486*/ "Wall/132",
  /*0487*/ "Floor/Lava5",
  /*0488*/ "Dummy",
  /*0489*/ "Wall/133",
  /*0490*/ "Wall/134",
  /*0491*/ "Wall/135",
  /*0492*/ "Wall/136",
  /*0493*/ "Wall/137",
  /*0494*/ "Wall/138",
  /*0495*/ "Wall/139",
  /*0496*/ "Characters/Rhian",
  /*0497*/ "Wall/Cursor01",
  /*0498*/ "Wall/Cursor02",
  /*0499*/ "Floor/Cursor04",
  /*0500*/ "GUI/M",
  /*0501*/ "Wall/Cursor03",
  /*0502*/ "Wall/140",
  /*0503*/ "Wall/Cursor04",
  /*0504*/ "Effects/Cracks4",
  /*0505*/ "Items/Switch1",
  /*0506*/ "Wall/Cursor05",
  /*0507*/ "Wall/141",
  /*0508*/ "GUI/5D",
  /*0509*/ "Units/Seeker",
  /*0510*/ "Floor/073",
  /*0511*/ "Effects/Lightning",
  /*0512*/ "Objects/58",
  /*0513*/ "Objects/59",
  /*0514*/ "Items/Garbage6",
  /*0515*/ "Objects/Door11",
  /*0516*/ "Objects/Door12",
  /*0517*/ "Objects/Door13",
  /*0518*/ "Objects/Door14",
  /*0519*/ "Floor/074",
  /*0520*/ "Floor/075",
  /*0521*/ "Floor/076",
  /*0522*/ "Effects/0522",
  /*0523*/ "PaperDoll/Torso",
  /*0524*/ "Objects/0524",
  /*0525*/ "Floor/CursorTrap",
  /*0526*/ "Objects/0526",
  /*0527*/ "Objects/0527",
  /*0528*/ "Objects/0528",
  /*0529*/ "Items/Backpack",
  /*0530*/ "PaperDoll/Armguards",
  /*0531*/ "PaperDoll/Helmets",
  /*0532*/ "PaperDoll/Leggings",
  /*0533*/ "Items/Rope",
  /*0534*/ "Items/0534",
  /*0535*/ "Items/0535",
  /*0536*/ "Items/0536",
  /*0537*/ "Items/Meat",
  /*0538*/ "Items/Food",
  /*0539*/ "PaperDoll/Shield",
  /*0540*/ "PaperDoll/Hammer",
  /*0541*/ "PaperDoll/Scimitar",
  /*0542*/ "PaperDoll/Saber",
  /*0543*/ "GUI/0543",
  /*0544*/ "Objects/0544",
  /*0545*/ "Objects/0545",
  /*0546*/ "Objects/0546",
  /*0547*/ "Objects/0547",
  /*0548*/ "Objects/0548",
  /*0549*/ "Floor/077",
  /*0550*/ "Objects/0549",
  /*0551*/ "Floor/078",
  /*0552*/ "Floor/079",
  /*0553*/ "Objects/0553",
  /*0554*/ "Items/0554",
  /*0555*/ "Items/0555",
  /*0556*/ "Floor/080",
  /*0557*/ "Floor/081",
  /*0558*/ "Floor/082",
  /*0559*/ "Floor/083",
  /*0560*/ "Wall/142",
  /*0561*/ "Wall/143",
  /*0562*/ "GUI/Music",
  /*0563*/ "Floor/084",
  /*0564*/ "Floor/085",
  /*0565*/ "Floor/086",
  /*0566*/ "Wall/144",
  /*0567*/ "Characters/MordreaBed",
  /*0568*/ "Items/Torch",
  /*0569*/ "Items/TorchFire",
  /*0570*/ "Characters/Hydros2",
  /*0571*/ "Items/CeremonialDagger",
  /*0572*/ "Characters/LothianStatic",
  /*0573*/ "Characters/Hydros3",
  /*0574*/ "Units/Guard",
  /*0575*/ "Wall/145",
  /*0576*/ "Wall/146",
  /*0577*/ "Items/Clotch",
  /*0578*/ "Effects/Explosion1",
  /*0579*/ "Items/OilFlask",
  /*0580*/ "Effects/Sparks",
  /*0581*/ "Units/GhoulSpawn",
  /*0582*/ "Items/0582",
  /*0583*/ "Floor/PushTile1",
  /*0584*/ "Objects/Nature17",
  /*0585*/ "Objects/Nature18",
  /*0586*/ "Items/0586",
  /*0587*/ "Effects/0587",
  /*0588*/ "Items/FishingNet",
  /*0589*/ "Items/FishingRod",
  /*0590*/ "Effects/0590",
  /*0591*/ "Effects/0591",
  /*0592*/ "Items/FireGem",
  /*0593*/ "Effects/0593",
  /*0594*/ "Effects/0594",
  /*0595*/ "Items/WritingItems",
  /*0596*/ "Items/Gems2",
  /*0597*/ "Objects/Statue05",
  /*0598*/ "Items/Plaque1",
  /*0599*/ "Items/Plaque2",
  /*0600*/ "Wall/147",
  /*0601*/ "Wall/148",
  /*0602*/ "Floor/087",
  /*0603*/ "Effects/LavaDrop3",
  /*0604*/ "Effects/LavaDrop4",
  /*0605*/ "Items/Candle4",
  /*0606*/ "Objects/0606",
  /*0607*/ "Items/Books03",
  /*0608*/ "Dummy",
  /*0609*/ "Floor/088",
  /*0610*/ "Floor/089",
  /*0611*/ "Floor/090",
  /*0612*/ "Wall/149",
  /*0613*/ "Items/Books04",
  /*0614*/ "Items/Plaque3",
  /*0615*/ "Objects/0615",
  /*0616*/ "Items/Bones07",
  /*0617*/ "Effects/WaterDrop5",
  /*0618*/ "Objects/0618",
  /*0619*/ "Wall/Cursor06",
  /*0620*/ "Effects/LavaDrop5",
  /*0621*/ "Dummy",
  /*0622*/ "Objects/RoofSpire1",
  /*0623*/ "Characters/AncientNecromancer",
  /*0624*/ "Items/Fish",
  /*0625*/ "Objects/Nature19",
  /*0626*/ "Objects/Nature20",
  /*0627*/ "Floor/091",
  /*0628*/ "Characters/LothianGrave",
  /*0629*/ "Items/0629",
  /*0630*/ "Effects/WindowProjection1",
  /*0631*/ "Effects/WindowProjection2",
  /*0632*/ "Effects/0632",
  /*0633*/ "Wall/150",
  /*0634*/ "Wall/Cursor07",
  /*0635*/ "Objects/Barrel",
  /*0636*/ "Floor/CursorReflect",
  /*0637*/ "Items/Bag",
  /*0638*/ "Objects/Boat",
  /*0639*/ "Wall/Cursor08",
  /*0640*/ "Objects/0640",
  /*0641*/ "Objects/0641",
  /*0642*/ "Wall/Bridge1",
  /*0643*/ "Objects/BoatPaddle",
  /*0644*/ "Floor/CursorOpenGround",
  /*0645*/ "Objects/0645",
  /*0646*/ "Objects/0646",
  /*0647*/ "Floor/092",
  /*0648*/ "Floor/093",
  /*0649*/ "Objects/Nature21",
  /*0650*/ "Objects/Nature22",
  /*0651*/ "Objects/Roof2",
  /*0652*/ "Objects/Roof3",
  /*0653*/ "Effects/WaterDrop6",
  /*0654*/ "Floor/094",
  /*0655*/ "Items/Wood01",
  /*0656*/ "Items/Wood02",
  /*0657*/ "Items/Wood03",
  /*0658*/ "Items/Wood04",
  /*0659*/ "Items/Wood05",
  /*0660*/ "Effects/Explosion2",
  /*0661*/ "Objects/Roof4",
  /*0662*/ "Wall/Bridge2",
  /*0663*/ "Items/0663",
  /*0664*/ "Objects/Well",
  /*0665*/ "Objects/0665",
  /*0666*/ "Items/Wood06",
  /*0667*/ "Items/Wood07",
  /*0668*/ "Floor/094",
  /*0669*/ "Floor/095",
  /*0670*/ "Floor/096",
  /*0671*/ "Floor/097",
  /*0672*/ "Items/Switch2",
  /*0673*/ "Objects/0673",
  /*0674*/ "Items/Wood08",
  /*0675*/ "Floor/WoodBridge1",
  /*0676*/ "Floor/WoodBridge2",
  /*0677*/ "Floor/WoodBridge3",
  /*0678*/ "Items/Wood09",
  /*0679*/ "Items/Wood10",
  /*0680*/ "Items/Wood11",
  /*0681*/ "Floor/WoodBridge4",
  /*0682*/ "Floor/WoodBridge5",
  /*0683*/ "Floor/Circle",
  /*0684*/ "Objects/Statue06",
  /*0685*/ "Objects/Crystal",
  /*0686*/ "Floor/Cursor05",
  /*0687*/ "Effects/0687",
  /*0688*/ "Floor/Cursor06",
  /*0689*/ "Floor/098",
  /*0690*/ "Objects/0690",
  /*0691*/ "Objects/0691",
  /*0692*/ "Floor/099",
  /*0693*/ "Floor/100",
  /*0694*/ "Items/Switch3",
  /*0695*/ "Items/Switch4",
  /*0696*/ "Items/FireTrap1",
  /*0697*/ "Items/FireTrapMissile",
  /*0698*/ "Effects/Cracks5",
  /*0699*/ "Effects/Cracks6",
  /*0700*/ "Objects/Statue07",
  /*0701*/ "Floor/101",
  /*0702*/ "Items/Target",
  /*0703*/ "Effects/Portal1",
  /*0704*/ "Wall/Cursor09",
  /*0705*/ "Wall/Cursor10",
  /*0706*/ "Items/FireTrap2",
  /*0707*/ "Characters/Peasant1",
  /*0708*/ "Characters/Boy",
  /*0709*/ "Effects/0709",
  /*0710*/ "Floor/110",
  /*0711*/ "Dummy",
  /*0712*/ "Items/Gems3",
  /*0713*/ "Wall/Cursor11",
  /*0714*/ "Wall/Cursor12",
  /*0715*/ "Floor/111",
  /*0716*/ "Floor/112",
  /*0717*/ "Floor/113",
  /*0718*/ "Floor/114",
  /*0719*/ "Items/Hay",
  /*0720*/ "Floor/115",
  /*0721*/ "Objects/RoofPipe1",
  /*0722*/ "Objects/RoofPipe2",
  /*0723*/ "Objects/RoofSpire2",
  /*0724*/ "Objects/RoofPipe3",
  /*0725*/ "Objects/0725",
  /*0726*/ "Floor/116",
  /*0727*/ "Floor/117",
  /*0728*/ "Wall/151",
  /*0729*/ "Wall/152",
  /*0730*/ "Wall/153",
  /*0731*/ "Items/TrainingDummy",
  /*0732*/ "Wall/154",
  /*0733*/ "Wall/155",
  /*0734*/ "Wall/156",
  /*0735*/ "Floor/Cursor07",
  /*0736*/ "Floor/Waterfall06",
  /*0737*/ "Floor/Waterfall07",
  /*0738*/ "Floor/Waterfall08",
  /*0739*/ "Items/FishBones",
  /*0740*/ "Floor/Cursor08",
  /*0741*/ "Floor/118",
  /*0742*/ "Floor/Cursor09",
  /*0743*/ "Dummy",
  /*0744*/ "Dummy",
  /*0745*/ "Floor/119",
  /*0746*/ "Items/NecromancySpells",
  /*0747*/ "Floor/Cursor10",
  /*0748*/ "Effects/Portal2",
  /*0749*/ "Effects/Portal3",
  /*0750*/ "Items/DeathDisc",
  /*0751*/ "Effects/WindowProjection3",
  /*0752*/ "Effects/WindowProjection4",
  /*0753*/ "Objects/Nature23",
  /*0754*/ "Items/Beds8",
  /*0755*/ "Floor/Lava6",
  /*0756*/ "GUI/Trap",
  /*0757*/ "Effects/0757",
  /*0758*/ "Effects/0758",
  /*0759*/ "Floor/120",
  /*0760*/ "Items/GemOfProtection",
  /*0761*/ "Floor/Cursor11",
  /*0762*/ "Wall/157",
  /*0763*/ "Dummy",
  /*0764*/ "Effects/StalactiteFall",
  /*0765*/ "Characters/Pyros8",
  /*0766*/ "Items/Potions",
  /*0767*/ "Floor/121",
  /*0768*/ "Floor/122",
  /*0769*/ "Characters/Avatar2",
  /*0770*/ "GUI/MoveArrows",
  /*0771*/ "Effects/StalagmiteEmerge1",
  /*0772*/ "Effects/Explosion3",
  /*0773*/ "Effects/StalagmiteEmerge2",
  /*0774*/ "Wall/158",
  /*0775*/ "Wall/159",
  /*0776*/ "Floor/123",
  /*0777*/ "Floor/124",
  /*0778*/ "Floor/Cursor12",
  /*0779*/ "Floor/125",
  /*0780*/ "Effects/0780",
  /*0781*/ "Floor/126",
  /*0782*/ "Floor/127",
  /*0783*/ "Floor/128",
  /*0784*/ "Items/ChaosGem",
  /*0785*/ "Items/Pole",
  /*0786*/ "Effects/Explosion4",
  /*0787*/ "Floor/Cursor13",
  /*0788*/ "Floor/129",
  /*0789*/ "Objects/Door15",
  /*0790*/ "Objects/Door16",
  /*0791*/ "Objects/Door17",
  /*0792*/ "Objects/Door18",
  /*0793*/ "Items/BlackrockObeliskParts",
  /*0794*/ "Objects/Nature24",
  /*0795*/ "Objects/0795",
  /*0796*/ "Objects/0796",
  /*0797*/ "Floor/Cursor14",
  /*0798*/ "Items/Rocks1",
  /*0799*/ "Objects/0799",
  /*0800*/ "Objects/0800",
  /*0801*/ "Objects/0801",
  /*0802*/ "Objects/0802",
  /*0803*/ "Objects/0803",
  /*0804*/ "Effects/SmallFireBolt",
  /*0805*/ "Wall/160",
  /*0806*/ "Units/SmallLizard",
  /*0807*/ "Units/SmallSpider",
  /*0808*/ "Units/Invisible",
  /*0809*/ "Items/MirrorsAndBrushes",
  /*0810*/ "Items/Figurine",
  /*0811*/ "Items/Dolls",
  /*0812*/ "Items/Plaques",
  /*0813*/ "Items/Fan",
  /*0814*/ "Items/SkullOfQuakes",
  /*0815*/ "PaperDoll/BoneCrusher",
  /*0816*/ "PaperDoll/Slayer",
  /*0817*/ "PaperDoll/FlameSting",
  /*0818*/ "PaperDoll/Protector",
  /*0819*/ "PaperDoll/Striking",
  /*0820*/ "PaperDoll/Korghin",
  /*0821*/ "PaperDoll/KumashGor",
  /*0822*/ "PaperDoll/Deciever",
  /*0823*/ "PaperDoll/Clotches",
  /*0824*/ "Units/SmallInsect",
  /*0825*/ "Effects/SpikesAnim",
  /*0826*/ "Effects/SpikesStatic",
  /*0827*/ "Items/Scrolls2",
  /*0828*/ "Items/ZealanCeremonialShield",
  /*0829*/ "Items/WashingItems",
  /*0830*/ "Items/0830",
  /*0831*/ "Items/0831",
  /*0832*/ "GUI/0832",
  /*0833*/ "Items/RecallItem",
  /*0834*/ "PaperDoll/Dagger2",
  /*0835*/ "Characters/Peasant2",
  /*0836*/ "Characters/Peasant3",
  /*0837*/ "Effects/0837",
  /*0838*/ "Items/Wood12",
  /*0839*/ "Effects/0839",
  /*0840*/ "Effects/0840",
  /*0841*/ "PaperDoll/MagicArmor2",
  /*0842*/ "PaperDoll/MagicShield",
  /*0843*/ "PaperDoll/MagicHelmet",
  /*0844*/ "PaperDoll/MagicArmguard",
  /*0845*/ "PaperDoll/MagicLeggings",
  /*0846*/ "Wall/Cursor13",
  /*0847*/ "Floor/PushTile2",
  /*0848*/ "Dummy",
  /*0849*/ "Dummy",
  /*0850*/ "Wall/Cursor14",
  /*0851*/ "Wall/161",
  /*0852*/ 0,
  /*0853*/ 0,
  /*0854*/ 0,
  /*0855*/ 0,
  /*0856*/ 0,
  /*0857*/ 0,
  /*0858*/ 0,
  /*0859*/ 0,
  /*0860*/ 0,
  /*0861*/ 0,
  /*0862*/ 0,
  /*0863*/ 0,
  /*0864*/ 0,
  /*0865*/ 0,
  /*0866*/ 0,
  /*0867*/ 0,
  /*0868*/ 0,
  /*0869*/ 0,
  /*0870*/ 0,
  /*0871*/ 0,
  /*0872*/ 0,
  /*0873*/ 0,
  /*0874*/ 0,
  /*0875*/ 0,
  /*0876*/ 0,
  /*0877*/ 0,
  /*0878*/ 0,
  /*0879*/ 0,
  /*0880*/ 0,
  /*0881*/ 0,
  /*0882*/ 0,
  /*0883*/ 0,
  /*0884*/ 0,
  /*0885*/ 0,
  /*0886*/ 0,
  /*0887*/ 0,
  /*0888*/ 0,
  /*0889*/ 0,
  /*0890*/ 0,
  /*0891*/ 0,
  /*0892*/ 0,
  /*0893*/ 0,
  /*0894*/ 0,
  /*0895*/ 0,
  /*0896*/ 0,
  /*0897*/ 0,
  /*0898*/ 0,
  /*0899*/ 0,
  /*0900*/ 0,
  /*0901*/ 0,
  /*0902*/ 0,
  /*0903*/ 0,
  /*0904*/ 0,
  /*0905*/ 0,
  /*0906*/ 0,
  /*0907*/ 0,
  /*0908*/ 0,
  /*0909*/ 0,
  /*0910*/ 0,
  /*0911*/ 0,
  /*0912*/ 0,
  /*0913*/ 0,
  /*0914*/ 0,
  /*0915*/ 0,
  /*0916*/ 0,
  /*0917*/ 0,
  /*0918*/ 0,
  /*0919*/ 0,
  /*0920*/ 0,
  /*0921*/ 0,
  /*0922*/ 0,
  /*0923*/ 0,
  /*0924*/ 0,
  /*0925*/ 0,
  /*0926*/ 0,
  /*0927*/ 0,
  /*0928*/ 0,
  /*0929*/ 0,
  /*0930*/ 0,
  /*0931*/ 0,
  /*0932*/ 0,
  /*0933*/ 0,
  /*0934*/ 0,
  /*0935*/ 0,
  /*0936*/ 0,
  /*0937*/ 0,
  /*0938*/ 0,
  /*0939*/ 0,
  /*0940*/ 0,
  /*0941*/ 0,
  /*0942*/ 0,
  /*0943*/ 0,
  /*0944*/ 0,
  /*0945*/ 0,
  /*0946*/ 0,
  /*0947*/ 0,
  /*0948*/ 0,
  /*0949*/ 0,
  /*0950*/ 0,
  /*0951*/ 0,
  /*0952*/ 0,
  /*0953*/ 0,
  /*0954*/ 0,
  /*0955*/ 0,
  /*0956*/ 0,
  /*0957*/ 0,
  /*0958*/ 0,
  /*0959*/ 0,
  /*0960*/ 0,
  /*0961*/ 0,
  /*0962*/ 0,
  /*0963*/ 0,
  /*0964*/ 0,
  /*0965*/ 0,
  /*0966*/ 0,
  /*0967*/ 0,
  /*0968*/ 0,
  /*0969*/ 0,
  /*0970*/ 0,
  /*0971*/ 0,
  /*0972*/ 0,
  /*0973*/ 0,
  /*0974*/ 0,
  /*0975*/ 0,
  /*0976*/ 0,
  /*0977*/ 0,
  /*0978*/ 0,
  /*0979*/ 0,
  /*0980*/ 0,
  /*0981*/ 0,
  /*0982*/ 0,
  /*0983*/ 0,
  /*0984*/ 0,
  /*0985*/ 0,
  /*0986*/ 0,
  /*0987*/ 0,
  /*0988*/ 0,
  /*0989*/ 0,
  /*0990*/ 0,
  /*0991*/ 0,
  /*0992*/ 0,
  /*0993*/ 0,
  /*0994*/ 0,
  /*0995*/ 0,
  /*0996*/ 0,
  /*0997*/ 0,
  /*0998*/ 0,
  /*0999*/ 0,
  /*1000*/ 0,
  /*1001*/ 0,
  /*1002*/ 0,
  /*1003*/ 0,
  /*1004*/ 0,
  /*1005*/ 0,
  /*1006*/ 0,
  /*1007*/ 0,
  /*1008*/ 0,
  /*1009*/ 0,
  /*1010*/ 0,
  /*1011*/ 0,
  /*1012*/ 0,
  /*1013*/ 0,
  /*1014*/ 0,
  /*1015*/ 0,
  /*1016*/ 0,
  /*1017*/ 0,
  /*1018*/ 0,
  /*1019*/ 0,
  /*1020*/ 0,
  /*1021*/ 0,
  /*1022*/ 0,
  /*1023*/ 0,
  /*1024*/ 0,
  /*1025*/ 0,
  /*1026*/ 0,
  /*1027*/ 0,
  /*1028*/ 0,
  /*1029*/ 0,
  /*1030*/ 0,
  /*1031*/ 0,
  /*1032*/ 0,
  /*1033*/ 0,
  /*1034*/ 0,
  /*1035*/ 0,
  /*1036*/ 0,
  /*1037*/ 0,
  /*1038*/ 0,
  /*1039*/ 0,
  /*1040*/ 0,
  /*1041*/ 0,
  /*1042*/ 0,
  /*1043*/ 0,
  /*1044*/ 0,
  /*1045*/ 0,
  /*1046*/ 0,
  /*1047*/ 0,
  /*1048*/ 0,
  /*1049*/ 0,
  /*1050*/ 0,
  /*1051*/ 0,
  /*1052*/ 0,
  /*1053*/ 0,
  /*1054*/ 0,
  /*1055*/ 0,
  /*1056*/ 0,
  /*1057*/ 0,
  /*1058*/ 0,
  /*1059*/ 0,
  /*1060*/ 0,
  /*1061*/ 0,
  /*1062*/ 0,
  /*1063*/ 0,
  /*1064*/ 0,
  /*1065*/ 0,
  /*1066*/ 0,
  /*1067*/ 0,
  /*1068*/ 0,
  /*1069*/ 0,
  /*1070*/ 0,
  /*1071*/ 0,
  /*1072*/ 0,
  /*1073*/ 0,
  /*1074*/ 0,
  /*1075*/ 0,
  /*1076*/ 0,
  /*1077*/ 0,
  /*1078*/ 0,
  /*1079*/ 0,
  /*1080*/ 0,
  /*1081*/ 0,
  /*1082*/ 0,
  /*1083*/ 0,
  /*1084*/ 0,
  /*1085*/ 0,
  /*1086*/ 0,
  /*1087*/ 0,
  /*1088*/ 0,
  /*1089*/ 0,
  /*1090*/ 0,
  /*1091*/ 0,
  /*1092*/ 0,
  /*1093*/ 0,
  /*1094*/ 0,
  /*1095*/ 0,
  /*1096*/ 0,
  /*1097*/ 0,
  /*1098*/ 0,
  /*1099*/ 0,
  /*1100*/ 0,
  /*1101*/ 0,
  /*1102*/ 0,
  /*1103*/ 0,
  /*1104*/ 0,
  /*1105*/ 0,
  /*1106*/ 0,
  /*1107*/ 0,
  /*1108*/ 0,
  /*1109*/ 0,
  /*1110*/ 0,
  /*1111*/ 0,
  /*1112*/ 0,
  /*1113*/ 0,
  /*1114*/ 0,
  /*1115*/ 0,
  /*1116*/ 0,
  /*1117*/ 0,
  /*1118*/ 0,
  /*1119*/ 0,
  /*1120*/ 0,
  /*1121*/ 0,
  /*1122*/ 0,
  /*1123*/ 0,
  /*1124*/ 0,
  /*1125*/ 0,
  /*1126*/ 0,
  /*1127*/ 0,
  /*1128*/ 0,
  /*1129*/ 0,
  /*1130*/ 0,
  /*1131*/ 0,
  /*1132*/ 0,
  /*1133*/ 0,
  /*1134*/ 0,
  /*1135*/ 0,
  /*1136*/ 0,
  /*1137*/ 0,
  /*1138*/ 0,
  /*1139*/ 0,
  /*1140*/ 0,
  /*1141*/ 0,
  /*1142*/ 0,
  /*1143*/ 0,
  /*1144*/ 0,
  /*1145*/ 0,
  /*1146*/ 0,
  /*1147*/ 0,
  /*1148*/ 0,
  /*1149*/ 0,
  /*1150*/ 0,
  /*1151*/ 0,
  /*1152*/ 0,
  /*1153*/ 0,
  /*1154*/ 0,
  /*1155*/ 0,
  /*1156*/ 0,
  /*1157*/ 0,
  /*1158*/ 0,
  /*1159*/ 0,
  /*1160*/ 0,
  /*1161*/ 0,
  /*1162*/ 0,
  /*1163*/ 0,
  /*1164*/ 0,
  /*1165*/ 0,
  /*1166*/ 0,
  /*1167*/ 0,
  /*1168*/ 0,
  /*1169*/ 0,
  /*1170*/ 0,
  /*1171*/ 0,
  /*1172*/ 0,
  /*1173*/ 0,
  /*1174*/ 0,
  /*1175*/ 0,
  /*1176*/ 0,
  /*1177*/ 0,
  /*1178*/ 0,
  /*1179*/ 0,
  /*1180*/ 0,
  /*1181*/ 0,
  /*1182*/ 0,
  /*1183*/ 0,
  /*1184*/ 0,
  /*1185*/ 0,
  /*1186*/ 0,
  /*1187*/ 0,
  /*1188*/ 0,
  /*1189*/ 0,
  /*1190*/ 0,
  /*1191*/ 0,
  /*1192*/ 0,
  /*1193*/ 0,
  /*1194*/ 0,
  /*1195*/ 0,
  /*1196*/ 0,
  /*1197*/ 0,
  /*1198*/ 0,
  /*1199*/ 0,
  /*1200*/ 0,
  /*1201*/ 0,
  /*1202*/ 0,
  /*1203*/ 0,
  /*1204*/ 0,
  /*1205*/ 0,
  /*1206*/ 0,
  /*1207*/ 0,
  /*1208*/ 0,
  /*1209*/ 0,
  /*1210*/ 0,
  /*1211*/ 0,
  /*1212*/ 0,
  /*1213*/ 0,
  /*1214*/ 0,
  /*1215*/ 0,
  /*1216*/ 0,
  /*1217*/ 0,
  /*1218*/ 0,
  /*1219*/ 0,
  /*1220*/ 0,
  /*1221*/ 0,
  /*1222*/ 0,
  /*1223*/ 0,
  /*1224*/ 0,
  /*1225*/ 0,
  /*1226*/ 0,
  /*1227*/ 0,
  /*1228*/ 0,
  /*1229*/ 0,
  /*1230*/ 0,
  /*1231*/ 0,
  /*1232*/ 0,
  /*1233*/ 0,
  /*1234*/ 0,
  /*1235*/ 0,
  /*1236*/ 0,
  /*1237*/ 0,
  /*1238*/ 0,
  /*1239*/ 0,
  /*1240*/ 0,
  /*1241*/ 0,
  /*1242*/ 0,
  /*1243*/ 0,
  /*1244*/ 0,
  /*1245*/ 0,
  /*1246*/ 0,
  /*1247*/ 0,
  /*1248*/ 0,
  /*1249*/ 0,
  /*1250*/ 0,
  /*1251*/ 0,
  /*1252*/ 0,
  /*1253*/ 0,
  /*1254*/ 0,
  /*1255*/ 0,
  /*1256*/ 0,
  /*1257*/ 0,
  /*1258*/ 0,
  /*1259*/ 0,
  /*1260*/ 0,
  /*1261*/ 0,
  /*1262*/ 0,
  /*1263*/ 0,
  /*1264*/ 0,
  /*1265*/ 0,
  /*1266*/ 0,
  /*1267*/ 0,
  /*1268*/ 0,
  /*1269*/ 0,
  /*1270*/ 0,
  /*1271*/ 0,
  /*1272*/ 0,
  /*1273*/ 0,
  /*1274*/ 0,
  /*1275*/ 0,
  /*1276*/ 0,
  /*1277*/ 0,
  /*1278*/ 0,
  /*1279*/ 0,
  /*1280*/ 0,
  /*1281*/ 0,
  /*1282*/ 0,
  /*1283*/ 0,
  /*1284*/ 0,
  /*1285*/ 0,
  /*1286*/ 0,
  /*1287*/ 0,
  /*1288*/ 0,
  /*1289*/ 0,
  /*1290*/ 0,
  /*1291*/ 0,
  /*1292*/ 0,
  /*1293*/ 0,
  /*1294*/ 0,
  /*1295*/ 0,
  /*1296*/ 0,
  /*1297*/ 0,
  /*1298*/ 0,
  /*1299*/ 0,
  /*1300*/ 0,
  /*1301*/ 0,
  /*1302*/ 0,
  /*1303*/ 0,
  /*1304*/ 0,
  /*1305*/ 0,
  /*1306*/ 0,
  /*1307*/ 0,
  /*1308*/ 0,
  /*1309*/ 0,
  /*1310*/ 0,
  /*1311*/ 0,
  /*1312*/ 0,
  /*1313*/ 0,
  /*1314*/ 0,
  /*1315*/ 0,
  /*1316*/ 0,
  /*1317*/ 0,
  /*1318*/ 0,
  /*1319*/ 0,
  /*1320*/ 0,
  /*1321*/ 0,
  /*1322*/ 0,
  /*1323*/ 0,
  /*1324*/ 0,
  /*1325*/ 0,
  /*1326*/ 0,
  /*1327*/ 0,
  /*1328*/ 0,
  /*1329*/ 0,
  /*1330*/ 0,
  /*1331*/ 0,
  /*1332*/ 0,
  /*1333*/ 0,
  /*1334*/ 0,
  /*1335*/ 0,
  /*1336*/ 0,
  /*1337*/ 0,
  /*1338*/ 0,
  /*1339*/ 0,
  /*1340*/ 0,
  /*1341*/ 0,
  /*1342*/ 0,
  /*1343*/ 0,
  /*1344*/ 0,
  /*1345*/ 0,
  /*1346*/ 0,
  /*1347*/ 0,
  /*1348*/ 0,
  /*1349*/ 0,
  /*1350*/ 0,
  /*1351*/ 0,
  /*1352*/ 0,
  /*1353*/ 0,
  /*1354*/ 0,
  /*1355*/ 0,
  /*1356*/ 0,
  /*1357*/ 0,
  /*1358*/ 0,
  /*1359*/ 0,
  /*1360*/ 0,
  /*1361*/ 0,
  /*1362*/ 0,
  /*1363*/ 0,
  /*1364*/ 0,
  /*1365*/ 0,
  /*1366*/ 0,
  /*1367*/ 0,
  /*1368*/ 0,
  /*1369*/ 0,
  /*1370*/ 0,
  /*1371*/ 0,
  /*1372*/ 0,
  /*1373*/ 0,
  /*1374*/ 0,
  /*1375*/ 0,
  /*1376*/ 0,
  /*1377*/ 0,
  /*1378*/ 0,
  /*1379*/ 0,
  /*1380*/ 0,
  /*1381*/ 0,
  /*1382*/ 0,
  /*1383*/ 0,
  /*1384*/ 0,
  /*1385*/ 0,
  /*1386*/ 0,
  /*1387*/ 0,
  /*1388*/ 0,
  /*1389*/ 0,
  /*1390*/ 0,
  /*1391*/ 0,
  /*1392*/ 0,
  /*1393*/ 0,
  /*1394*/ 0,
  /*1395*/ 0,
  /*1396*/ 0,
  /*1397*/ 0,
  /*1398*/ 0,
  /*1399*/ 0,
  /*1400*/ 0,
  /*1401*/ 0,
  /*1402*/ 0,
  /*1403*/ 0,
  /*1404*/ 0,
  /*1405*/ 0,
  /*1406*/ 0,
  /*1407*/ 0,
  /*1408*/ 0,
  /*1409*/ 0,
  /*1410*/ 0,
  /*1411*/ 0,
  /*1412*/ 0,
  /*1413*/ 0,
  /*1414*/ 0,
  /*1415*/ 0,
  /*1416*/ 0,
  /*1417*/ 0,
  /*1418*/ 0,
  /*1419*/ 0,
  /*1420*/ 0,
  /*1421*/ 0,
  /*1422*/ 0,
  /*1423*/ 0,
  /*1424*/ 0,
  /*1425*/ 0,
  /*1426*/ 0,
  /*1427*/ 0,
  /*1428*/ 0,
  /*1429*/ 0,
  /*1430*/ 0,
  /*1431*/ 0,
  /*1432*/ 0,
  /*1433*/ 0,
  /*1434*/ 0,
  /*1435*/ 0,
  /*1436*/ 0,
  /*1437*/ 0,
  /*1438*/ 0,
  /*1439*/ 0,
  /*1440*/ 0,
  /*1441*/ 0,
  /*1442*/ 0,
  /*1443*/ 0,
  /*1444*/ 0,
  /*1445*/ 0,
  /*1446*/ 0,
  /*1447*/ 0,
  /*1448*/ 0,
  /*1449*/ 0,
  /*1450*/ 0,
  /*1451*/ 0,
  /*1452*/ 0,
  /*1453*/ 0,
  /*1454*/ 0,
  /*1455*/ 0,
  /*1456*/ 0,
  /*1457*/ 0,
  /*1458*/ 0,
  /*1459*/ 0,
  /*1460*/ 0,
  /*1461*/ 0,
  /*1462*/ 0,
  /*1463*/ 0,
  /*1464*/ 0,
  /*1465*/ 0,
  /*1466*/ 0,
  /*1467*/ 0,
  /*1468*/ 0,
  /*1469*/ 0,
  /*1470*/ 0,
  /*1471*/ 0,
  /*1472*/ 0,
  /*1473*/ 0,
  /*1474*/ 0,
  /*1475*/ 0,
  /*1476*/ 0,
  /*1477*/ 0,
  /*1478*/ 0,
  /*1479*/ 0,
  /*1480*/ 0,
  /*1481*/ 0,
  /*1482*/ 0,
  /*1483*/ 0,
  /*1484*/ 0,
  /*1485*/ 0,
  /*1486*/ 0,
  /*1487*/ 0,
  /*1488*/ 0,
  /*1489*/ 0,
  /*1490*/ 0,
  /*1491*/ 0,
  /*1492*/ 0,
  /*1493*/ 0,
  /*1494*/ 0,
  /*1495*/ 0,
  /*1496*/ 0,
  /*1497*/ 0,
  /*1498*/ 0,
  /*1499*/ 0,
  /*1500*/ 0,
  /*1501*/ 0,
  /*1502*/ 0,
  /*1503*/ 0,
  /*1504*/ 0,
  /*1505*/ 0,
  /*1506*/ 0,
  /*1507*/ 0,
  /*1508*/ 0,
  /*1509*/ 0,
  /*1510*/ 0,
  /*1511*/ 0,
  /*1512*/ 0,
  /*1513*/ 0,
  /*1514*/ 0,
  /*1515*/ 0,
  /*1516*/ 0,
  /*1517*/ 0,
  /*1518*/ 0,
  /*1519*/ 0,
  /*1520*/ 0,
  /*1521*/ 0,
  /*1522*/ 0,
  /*1523*/ 0,
  /*1524*/ 0,
  /*1525*/ 0,
  /*1526*/ 0,
  /*1527*/ 0,
  /*1528*/ 0,
  /*1529*/ 0,
  /*1530*/ 0,
  /*1531*/ 0,
  /*1532*/ 0,
  /*1533*/ 0,
  /*1534*/ 0,
  /*1535*/ 0,
  /*1536*/ 0,
  /*1537*/ 0,
  /*1538*/ 0,
  /*1539*/ 0,
  /*1540*/ 0,
  /*1541*/ 0,
  /*1542*/ 0,
  /*1543*/ 0,
  /*1544*/ 0,
  /*1545*/ 0,
  /*1546*/ 0,
  /*1547*/ 0,
  /*1548*/ 0,
  /*1549*/ 0,
  /*1550*/ 0,
  /*1551*/ 0,
  /*1552*/ 0,
  /*1553*/ 0,
  /*1554*/ 0,
  /*1555*/ 0,
  /*1556*/ 0,
  /*1557*/ 0,
  /*1558*/ 0,
  /*1559*/ 0,
  /*1560*/ 0,
  /*1561*/ 0,
  /*1562*/ 0,
  /*1563*/ 0,
  /*1564*/ 0,
  /*1565*/ 0,
  /*1566*/ 0,
  /*1567*/ 0,
  /*1568*/ 0,
  /*1569*/ 0,
  /*1570*/ 0,
  /*1571*/ 0,
  /*1572*/ 0,
  /*1573*/ 0,
  /*1574*/ 0,
  /*1575*/ 0,
  /*1576*/ 0,
  /*1577*/ 0,
  /*1578*/ 0,
  /*1579*/ 0,
  /*1580*/ 0,
  /*1581*/ 0,
  /*1582*/ 0,
  /*1583*/ 0,
  /*1584*/ 0,
  /*1585*/ 0,
  /*1586*/ 0,
  /*1587*/ 0,
  /*1588*/ 0,
  /*1589*/ 0,
  /*1590*/ 0,
  /*1591*/ 0,
  /*1592*/ 0,
  /*1593*/ 0,
  /*1594*/ 0,
  /*1595*/ 0,
  /*1596*/ 0,
  /*1597*/ 0,
  /*1598*/ 0,
  /*1599*/ 0,
  /*1600*/ 0,
  /*1601*/ 0,
  /*1602*/ 0,
  /*1603*/ 0,
  /*1604*/ 0,
  /*1605*/ 0,
  /*1606*/ 0,
  /*1607*/ 0,
  /*1608*/ 0,
  /*1609*/ 0,
  /*1610*/ 0,
  /*1611*/ 0,
  /*1612*/ 0,
  /*1613*/ 0,
  /*1614*/ 0,
  /*1615*/ 0,
  /*1616*/ 0,
  /*1617*/ 0,
  /*1618*/ 0,
  /*1619*/ 0,
  /*1620*/ 0,
  /*1621*/ 0,
  /*1622*/ 0,
  /*1623*/ 0,
  /*1624*/ 0,
  /*1625*/ 0,
  /*1626*/ 0,
  /*1627*/ 0,
  /*1628*/ 0,
  /*1629*/ 0,
  /*1630*/ 0,
  /*1631*/ 0,
  /*1632*/ 0,
  /*1633*/ 0,
  /*1634*/ 0,
  /*1635*/ 0,
  /*1636*/ 0,
  /*1637*/ 0,
  /*1638*/ 0,
  /*1639*/ 0,
  /*1640*/ 0,
  /*1641*/ 0,
  /*1642*/ 0,
  /*1643*/ 0,
  /*1644*/ 0,
  /*1645*/ 0,
  /*1646*/ 0,
  /*1647*/ 0,
  /*1648*/ 0,
  /*1649*/ 0,
  /*1650*/ 0,
  /*1651*/ 0,
  /*1652*/ 0,
  /*1653*/ 0,
  /*1654*/ 0,
  /*1655*/ 0,
  /*1656*/ 0,
  /*1657*/ 0,
  /*1658*/ 0,
  /*1659*/ 0,
  /*1660*/ 0,
  /*1661*/ 0,
  /*1662*/ 0,
  /*1663*/ 0,
  /*1664*/ 0,
  /*1665*/ 0,
  /*1666*/ 0,
  /*1667*/ 0,
  /*1668*/ 0,
  /*1669*/ 0,
  /*1670*/ 0,
  /*1671*/ 0,
  /*1672*/ 0,
  /*1673*/ 0,
  /*1674*/ 0,
  /*1675*/ 0,
  /*1676*/ 0,
  /*1677*/ 0,
  /*1678*/ 0,
  /*1679*/ 0,
  /*1680*/ 0,
  /*1681*/ 0,
  /*1682*/ 0,
  /*1683*/ 0,
  /*1684*/ 0,
  /*1685*/ 0,
  /*1686*/ 0,
  /*1687*/ 0,
  /*1688*/ 0,
  /*1689*/ 0,
  /*1690*/ 0,
  /*1691*/ 0,
  /*1692*/ 0,
  /*1693*/ 0,
  /*1694*/ 0,
  /*1695*/ 0,
  /*1696*/ 0,
  /*1697*/ 0,
  /*1698*/ 0,
  /*1699*/ 0,
  /*1700*/ 0,
  /*1701*/ 0,
  /*1702*/ 0,
  /*1703*/ 0,
  /*1704*/ 0,
  /*1705*/ 0,
  /*1706*/ 0,
  /*1707*/ 0,
  /*1708*/ 0,
  /*1709*/ 0,
  /*1710*/ 0,
  /*1711*/ 0,
  /*1712*/ 0,
  /*1713*/ 0,
  /*1714*/ 0,
  /*1715*/ 0,
  /*1716*/ 0,
  /*1717*/ 0,
  /*1718*/ 0,
  /*1719*/ 0,
  /*1720*/ 0,
  /*1721*/ 0,
  /*1722*/ 0,
  /*1723*/ 0,
  /*1724*/ 0,
  /*1725*/ 0,
  /*1726*/ 0,
  /*1727*/ 0,
  /*1728*/ 0,
  /*1729*/ 0,
  /*1730*/ 0,
  /*1731*/ 0,
  /*1732*/ 0,
  /*1733*/ 0,
  /*1734*/ 0,
  /*1735*/ 0,
  /*1736*/ 0,
  /*1737*/ 0,
  /*1738*/ 0,
  /*1739*/ 0,
  /*1740*/ 0,
  /*1741*/ 0,
  /*1742*/ 0,
  /*1743*/ 0,
  /*1744*/ 0,
  /*1745*/ 0,
  /*1746*/ 0,
  /*1747*/ 0,
  /*1748*/ 0,
  /*1749*/ 0,
  /*1750*/ 0,
  /*1751*/ 0,
  /*1752*/ 0,
  /*1753*/ 0,
  /*1754*/ 0,
  /*1755*/ 0,
  /*1756*/ 0,
  /*1757*/ 0,
  /*1758*/ 0,
  /*1759*/ 0,
  /*1760*/ 0,
  /*1761*/ 0,
  /*1762*/ 0,
  /*1763*/ 0,
  /*1764*/ 0,
  /*1765*/ 0,
  /*1766*/ 0,
  /*1767*/ 0,
  /*1768*/ 0,
  /*1769*/ 0,
  /*1770*/ 0,
  /*1771*/ 0,
  /*1772*/ 0,
  /*1773*/ 0,
  /*1774*/ 0,
  /*1775*/ 0,
  /*1776*/ 0,
  /*1777*/ 0,
  /*1778*/ 0,
  /*1779*/ 0,
  /*1780*/ 0,
  /*1781*/ 0,
  /*1782*/ 0,
  /*1783*/ 0,
  /*1784*/ 0,
  /*1785*/ 0,
  /*1786*/ 0,
  /*1787*/ 0,
  /*1788*/ 0,
  /*1789*/ 0,
  /*1790*/ 0,
  /*1791*/ 0,
  /*1792*/ 0,
  /*1793*/ 0,
  /*1794*/ 0,
  /*1795*/ 0,
  /*1796*/ 0,
  /*1797*/ 0,
  /*1798*/ 0,
  /*1799*/ 0,
  /*1800*/ 0,
  /*1801*/ 0,
  /*1802*/ 0,
  /*1803*/ 0,
  /*1804*/ 0,
  /*1805*/ 0,
  /*1806*/ 0,
  /*1807*/ 0,
  /*1808*/ 0,
  /*1809*/ 0,
  /*1810*/ 0,
  /*1811*/ 0,
  /*1812*/ 0,
  /*1813*/ 0,
  /*1814*/ 0,
  /*1815*/ 0,
  /*1816*/ 0,
  /*1817*/ 0,
  /*1818*/ 0,
  /*1819*/ 0,
  /*1820*/ 0,
  /*1821*/ 0,
  /*1822*/ 0,
  /*1823*/ 0,
  /*1824*/ 0,
  /*1825*/ 0,
  /*1826*/ 0,
  /*1827*/ 0,
  /*1828*/ 0,
  /*1829*/ 0,
  /*1830*/ 0,
  /*1831*/ 0,
  /*1832*/ 0,
  /*1833*/ 0,
  /*1834*/ 0,
  /*1835*/ 0,
  /*1836*/ 0,
  /*1837*/ 0,
  /*1838*/ 0,
  /*1839*/ 0,
  /*1840*/ 0,
  /*1841*/ 0,
  /*1842*/ 0,
  /*1843*/ 0,
  /*1844*/ 0,
  /*1845*/ 0,
  /*1846*/ 0,
  /*1847*/ 0,
  /*1848*/ 0,
  /*1849*/ 0,
  /*1850*/ 0,
  /*1851*/ 0,
  /*1852*/ 0,
  /*1853*/ 0,
  /*1854*/ 0,
  /*1855*/ 0,
  /*1856*/ 0,
  /*1857*/ 0,
  /*1858*/ 0,
  /*1859*/ 0,
  /*1860*/ 0,
  /*1861*/ 0,
  /*1862*/ 0,
  /*1863*/ 0,
  /*1864*/ 0,
  /*1865*/ 0,
  /*1866*/ 0,
  /*1867*/ 0,
  /*1868*/ 0,
  /*1869*/ 0,
  /*1870*/ 0,
  /*1871*/ 0,
  /*1872*/ 0,
  /*1873*/ 0,
  /*1874*/ 0,
  /*1875*/ 0,
  /*1876*/ 0,
  /*1877*/ 0,
  /*1878*/ 0,
  /*1879*/ 0,
  /*1880*/ 0,
  /*1881*/ 0,
  /*1882*/ 0,
  /*1883*/ 0,
  /*1884*/ 0,
  /*1885*/ 0,
  /*1886*/ 0,
  /*1887*/ 0,
  /*1888*/ 0,
  /*1889*/ 0,
  /*1890*/ 0,
  /*1891*/ 0,
  /*1892*/ 0,
  /*1893*/ 0,
  /*1894*/ 0,
  /*1895*/ 0,
  /*1896*/ 0,
  /*1897*/ 0,
  /*1898*/ 0,
  /*1899*/ 0,
  /*1900*/ 0,
  /*1901*/ 0,
  /*1902*/ 0,
  /*1903*/ 0,
  /*1904*/ 0,
  /*1905*/ 0,
  /*1906*/ 0,
  /*1907*/ 0,
  /*1908*/ 0,
  /*1909*/ 0,
  /*1910*/ 0,
  /*1911*/ 0,
  /*1912*/ 0,
  /*1913*/ 0,
  /*1914*/ 0,
  /*1915*/ 0,
  /*1916*/ 0,
  /*1917*/ 0,
  /*1918*/ 0,
  /*1919*/ 0,
  /*1920*/ 0,
  /*1921*/ 0,
  /*1922*/ 0,
  /*1923*/ 0,
  /*1924*/ 0,
  /*1925*/ 0,
  /*1926*/ 0,
  /*1927*/ 0,
  /*1928*/ 0,
  /*1929*/ 0,
  /*1930*/ 0,
  /*1931*/ 0,
  /*1932*/ 0,
  /*1933*/ 0,
  /*1934*/ 0,
  /*1935*/ 0,
  /*1936*/ 0,
  /*1937*/ 0,
  /*1938*/ 0,
  /*1939*/ 0,
  /*1940*/ 0,
  /*1941*/ 0,
  /*1942*/ 0,
  /*1943*/ 0,
  /*1944*/ 0,
  /*1945*/ 0,
  /*1946*/ 0,
  /*1947*/ 0,
  /*1948*/ 0,
  /*1949*/ 0,
  /*1950*/ 0,
  /*1951*/ 0,
  /*1952*/ 0,
  /*1953*/ 0,
  /*1954*/ 0,
  /*1955*/ 0,
  /*1956*/ 0,
  /*1957*/ 0,
  /*1958*/ 0,
  /*1959*/ 0,
  /*1960*/ 0,
  /*1961*/ 0,
  /*1962*/ 0,
  /*1963*/ 0,
  /*1964*/ 0,
  /*1965*/ 0,
  /*1966*/ 0,
  /*1967*/ 0,
  /*1968*/ 0,
  /*1969*/ 0,
  /*1970*/ 0,
  /*1971*/ 0,
  /*1972*/ 0,
  /*1973*/ 0,
  /*1974*/ 0,
  /*1975*/ 0,
  /*1976*/ 0,
  /*1977*/ 0,
  /*1978*/ 0,
  /*1979*/ 0,
  /*1980*/ 0,
  /*1981*/ 0,
  /*1982*/ 0,
  /*1983*/ 0,
  /*1984*/ 0,
  /*1985*/ 0,
  /*1986*/ 0,
  /*1987*/ 0,
  /*1988*/ 0,
  /*1989*/ 0,
  /*1990*/ 0,
  /*1991*/ 0,
  /*1992*/ 0,
  /*1993*/ 0,
  /*1994*/ 0,
  /*1995*/ 0,
  /*1996*/ 0,
  /*1997*/ 0,
  /*1998*/ 0,
  /*1999*/ 0,
  /*2000*/ 0,
  /*2001*/ 0,
  /*2002*/ 0,
  /*2003*/ 0,
  /*2004*/ 0,
  /*2005*/ 0,
  /*2006*/ 0,
  /*2007*/ 0,
  /*2008*/ 0,
  /*2009*/ 0,
  /*2010*/ 0,
  /*2011*/ 0,
  /*2012*/ 0,
  /*2013*/ 0,
  /*2014*/ 0,
  /*2015*/ 0,
  /*2016*/ 0,
  /*2017*/ 0,
  /*2018*/ 0,
  /*2019*/ 0,
  /*2020*/ 0,
  /*2021*/ 0,
  /*2022*/ 0,
  /*2023*/ 0,
  /*2024*/ 0,
  /*2025*/ 0,
  /*2026*/ 0,
  /*2027*/ 0,
  /*2028*/ 0,
  /*2029*/ 0,
  /*2030*/ 0,
  /*2031*/ 0,
  /*2032*/ 0,
  /*2033*/ 0,
  /*2034*/ 0,
  /*2035*/ 0,
  /*2036*/ 0,
  /*2037*/ 0,
  /*2038*/ 0,
  /*2039*/ 0,
  /*2040*/ 0,
  /*2041*/ 0,
  /*2042*/ 0,
  /*2043*/ 0,
  /*2044*/ 0,
  /*2045*/ 0,
  /*2046*/ 0,
  /*2047*/ 0};


static u1 U7_Pal[] = {
 0x00, 0x00, 0x00, 0xFB, 0xF3, 0xCF, 0xF7, 0xE7,
 0xA6, 0xF3, 0xDF, 0x79, 0xEF, 0xD3, 0x51, 0xEF,
 0xCB, 0x28, 0xDB, 0xAE, 0x20, 0xC7, 0x96, 0x18,
 0xB2, 0x82, 0x10, 0x9E, 0x69, 0x0C, 0x8A, 0x55,
 0x08, 0x75, 0x45, 0x04, 0x61, 0x30, 0x00, 0x4D,
 0x24, 0x00, 0x38, 0x14, 0x00, 0xFF, 0xFF, 0xFF,
 0xFF, 0xDB, 0xDB, 0xFF, 0xBA, 0xBA, 0xFF, 0x9A,
 0x9E, 0xFF, 0x79, 0x82, 0xFF, 0x59, 0x65, 0xFF,
 0x38, 0x4D, 0xFF, 0x1C, 0x34, 0xDF, 0x14, 0x28,
 0xC3, 0x0C, 0x1C, 0xA6, 0x08, 0x14, 0x8A, 0x04,
 0x0C, 0x6D, 0x00, 0x04, 0x51, 0x00, 0x00, 0x34,
 0x00, 0x00, 0x18, 0x00, 0x00, 0xFF, 0xEF, 0xDB,
 0xFF, 0xDF, 0xBA, 0xFF, 0xCF, 0x9A, 0xFF, 0xBE,
 0x7D, 0xFF, 0xAE, 0x5D, 0xFF, 0x9E, 0x3C, 0xFF,
 0x8E, 0x1C, 0xFF, 0x7D, 0x00, 0xE3, 0x6D, 0x00,
 0xC3, 0x61, 0x00, 0xA6, 0x51, 0x00, 0x8A, 0x45,
 0x00, 0x6D, 0x34, 0x00, 0x51, 0x24, 0x00, 0x34,
 0x18, 0x00, 0x18, 0x08, 0x00, 0xFF, 0xFF, 0xDB,
 0xF7, 0xF7, 0x9E, 0xEF, 0xEF, 0x61, 0xE7, 0xE7,
 0x2C, 0xDF, 0xDF, 0x00, 0xC3, 0xC3, 0x00, 0xA6,
 0xA6, 0x00, 0x8A, 0x8A, 0x00, 0x6D, 0x6D, 0x00,
 0x51, 0x51, 0x00, 0x34, 0x34, 0x00, 0x18, 0x18,
 0x00, 0xDB, 0xFF, 0xDB, 0xB2, 0xFF, 0xAE, 0x8E,
 0xFF, 0x82, 0x6D, 0xFF, 0x55, 0x51, 0xFF, 0x28,
 0x38, 0xFF, 0x00, 0x28, 0xDF, 0x00, 0x1C, 0xC3,
 0x00, 0x14, 0xA6, 0x00, 0x0C, 0x8A, 0x00, 0x04,
 0x6D, 0x00, 0x00, 0x51, 0x00, 0x00, 0x34, 0x00,
 0x00, 0x18, 0x00, 0xDB, 0xDB, 0xFF, 0xBA, 0xBA,
 0xFF, 0x9A, 0x9A, 0xFF, 0x7D, 0x7D, 0xFF, 0x5D,
 0x5D, 0xFF, 0x3C, 0x3C, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0xE3, 0x00, 0x00, 0xC3, 0x00, 0x00,
 0xA6, 0x00, 0x00, 0x8A, 0x00, 0x00, 0x6D, 0x00,
 0x00, 0x51, 0x00, 0x00, 0x34, 0x00, 0x00, 0x18,
 0xEB, 0xCB, 0xEB, 0xD7, 0x9A, 0xD7, 0xC7, 0x6D,
 0xC7, 0xB2, 0x49, 0xB2, 0xA2, 0x28, 0xA2, 0x8E,
 0x10, 0x8E, 0x7D, 0x00, 0x7D, 0x6D, 0x00, 0x6D,
 0x61, 0x00, 0x61, 0x51, 0x00, 0x51, 0x45, 0x00,
 0x45, 0x34, 0x00, 0x34, 0x24, 0x00, 0x24, 0x18,
 0x00, 0x18, 0xF7, 0xEB, 0xE7, 0xEF, 0xDF, 0xD7,
 0xE7, 0xCF, 0xC3, 0xE3, 0xC3, 0xB2, 0xDB, 0xB2,
 0xA2, 0xD3, 0xA6, 0x92, 0xCB, 0x9A, 0x82, 0xC7,
 0x8E, 0x75, 0xAE, 0x7D, 0x65, 0x9A, 0x6D, 0x59,
 0x82, 0x5D, 0x4D, 0x6D, 0x4D, 0x3C, 0x55, 0x3C,
 0x30, 0x3C, 0x2C, 0x24, 0x28, 0x1C, 0x14, 0x10,
 0x0C, 0x08, 0xEF, 0xEF, 0xEF, 0xDF, 0xDF, 0xDF,
 0xCF, 0xCF, 0xCF, 0xBE, 0xBE, 0xBE, 0xAE, 0xAE,
 0xAE, 0x9E, 0x9E, 0x9E, 0x8E, 0x8E, 0x8E, 0x7D,
 0x7D, 0x7D, 0x6D, 0x6D, 0x6D, 0x61, 0x61, 0x61,
 0x51, 0x51, 0x51, 0x45, 0x45, 0x45, 0x34, 0x34,
 0x34, 0x24, 0x24, 0x24, 0x18, 0x18, 0x18, 0x08,
 0x08, 0x08, 0xEB, 0xE3, 0xD7, 0xDB, 0xCB, 0xB2,
 0xCB, 0xB2, 0x92, 0xBA, 0x9A, 0x71, 0xAA, 0x86,
 0x59, 0x9A, 0x71, 0x41, 0x8A, 0x5D, 0x2C, 0x7D,
 0x4D, 0x18, 0x6D, 0x3C, 0x0C, 0x5D, 0x34, 0x0C,
 0x4D, 0x2C, 0x0C, 0x3C, 0x24, 0x0C, 0x2C, 0x1C,
 0x08, 0x20, 0x14, 0x08, 0xEF, 0xEB, 0xE7, 0xDF,
 0xD7, 0xD3, 0xCF, 0xC7, 0xBE, 0xBE, 0xB2, 0xAE,
 0xAE, 0xA2, 0x9A, 0x9E, 0x92, 0x8A, 0x8E, 0x82,
 0x79, 0x7D, 0x71, 0x69, 0x6D, 0x61, 0x5D, 0x61,
 0x55, 0x51, 0x51, 0x49, 0x45, 0x45, 0x3C, 0x38,
 0x34, 0x30, 0x2C, 0x24, 0x20, 0x20, 0x18, 0x14,
 0x14, 0xE3, 0xEB, 0xD7, 0xCB, 0xD7, 0xB6, 0xB6,
 0xC3, 0x9A, 0x9E, 0xAE, 0x7D, 0x8A, 0x9A, 0x61,
 0x71, 0x86, 0x4D, 0x5D, 0x71, 0x38, 0x4D, 0x5D,
 0x28, 0x41, 0x51, 0x20, 0x38, 0x45, 0x1C, 0x30,
 0x3C, 0x18, 0x28, 0x30, 0x14, 0x20, 0x24, 0x10,
 0x18, 0x1C, 0x08, 0x0C, 0x10, 0x04, 0xEF, 0xDB,
 0xCF, 0xDF, 0xBA, 0xA2, 0xCF, 0x9A, 0x7D, 0xBE,
 0x82, 0x5D, 0xAE, 0x65, 0x3C, 0x9E, 0x51, 0x24,
 0x8E, 0x3C, 0x0C, 0x7D, 0x2C, 0x00, 0x6D, 0x24,
 0x00, 0x61, 0x20, 0x00, 0x51, 0x1C, 0x00, 0x45,
 0x14, 0x00, 0x34, 0x10, 0x00, 0x24, 0x0C, 0x00,
 0xF3, 0xF3, 0xFF, 0xE7, 0xE7, 0xFF, 0xDB, 0xDB,
 0xFF, 0xCF, 0xCF, 0xFF, 0xC3, 0xC3, 0xFF, 0xB6,
 0xB6, 0xFF, 0xAA, 0xAA, 0xFF, 0x9E, 0x9E, 0xFF,
 0x86, 0xD3, 0x00, 0x86, 0xB2, 0x00, 0x7D, 0x96,
 0x00, 0x69, 0x79, 0x00, 0x51, 0x59, 0x00, 0x3C,
 0x41, 0x00, 0x2C, 0x24, 0x00, 0x1C, 0x08, 0x00,
 0x20, 0x00, 0x00, 0xEF, 0xDB, 0xC7, 0xDF, 0xC3,
 0xB6, 0xCF, 0xB6, 0xA2, 0xBE, 0x9E, 0x96, 0xAE,
 0x92, 0x82, 0x9E, 0x86, 0x75, 0x8E, 0x75, 0x65,
 0x7D, 0x65, 0x59, 0x6D, 0x55, 0x4D, 0x61, 0x49,
 0x45, 0x51, 0x41, 0x38, 0x45, 0x34, 0x2C, 0x34,
 0x2C, 0x24, 0x24, 0x18, 0x18, 0x18, 0x10, 0x10,
 0xB2, 0xD7, 0xF3, 0xFF, 0xFF, 0xFF, 0xB2, 0xD7,
 0xF3, 0x71, 0xAE, 0xE7, 0x34, 0x8E, 0xDB, 0x00,
 0x6D, 0xD3, 0x30, 0x8E, 0xDB, 0x6D, 0xB2, 0xE7,
 0x69, 0x18, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF,
 0x41, 0xFF, 0xC3, 0x28, 0xFF, 0x8E, 0x10, 0xFF,
 0x51, 0x00, 0xCB, 0x38, 0x00, 0x9A, 0x28, 0x00,
 0x00, 0x6D, 0x00, 0x7D, 0xDF, 0x7D, 0x45, 0xB6,
 0x45, 0x18, 0x92, 0x18, 0xCF, 0x00, 0x71, 0xFB,
 0xBA, 0xFF, 0xFF, 0x65, 0xEF, 0xFF, 0x00, 0xB6,
 0xFF, 0xAE, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
 0x00, 0xFF, 0xD3, 0x00, 0xFF, 0xD3, 0x71, 0x00,
 0x00, 0x00, 0x65, 0x3C, 0x10, 0xEB, 0x00, 0x04};


static u1 U8_Pal[] = {
 0x00, 0x00, 0x00, 0x00, 0x3E, 0x3E, 0x3F, 0x00,
 0x3A, 0x3A, 0x3F, 0x00, 0x37, 0x37, 0x3F, 0x00,
 0x34, 0x34, 0x3F, 0x00, 0x30, 0x30, 0x3F, 0x00,
 0x2D, 0x2D, 0x3F, 0x00, 0x2A, 0x2A, 0x3F, 0x00,
 0x13, 0x3F, 0x12, 0x00, 0x0C, 0x0C, 0x0C, 0x00,
 0x3F, 0x3F, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x00,
 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00,
 0x00, 0x00, 0x1A, 0x00, 0x3F, 0x3F, 0x3F, 0x00,
 0x3B, 0x3B, 0x3B, 0x00, 0x37, 0x37, 0x37, 0x00,
 0x33, 0x33, 0x33, 0x00, 0x2F, 0x2F, 0x2F, 0x00,
 0x2B, 0x2B, 0x2B, 0x00, 0x27, 0x27, 0x27, 0x00,
 0x23, 0x23, 0x23, 0x00, 0x20, 0x20, 0x20, 0x00,
 0x1C, 0x1C, 0x1C, 0x00, 0x18, 0x18, 0x18, 0x00,
 0x14, 0x14, 0x14, 0x00, 0x10, 0x10, 0x10, 0x00,
 0x0C, 0x0C, 0x0C, 0x00, 0x08, 0x08, 0x08, 0x00,
 0x04, 0x04, 0x04, 0x00, 0x01, 0x01, 0x01, 0x00,
 0x3F, 0x3B, 0x3B, 0x00, 0x3D, 0x30, 0x30, 0x00,
 0x3B, 0x26, 0x26, 0x00, 0x39, 0x1D, 0x1D, 0x00,
 0x37, 0x14, 0x14, 0x00, 0x35, 0x0C, 0x0C, 0x00,
 0x34, 0x04, 0x04, 0x00, 0x2F, 0x03, 0x03, 0x00,
 0x2A, 0x02, 0x02, 0x00, 0x26, 0x01, 0x01, 0x00,
 0x21, 0x01, 0x01, 0x00, 0x1C, 0x00, 0x00, 0x00,
 0x18, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
 0x3F, 0x3F, 0x3E, 0x00, 0x3F, 0x3F, 0x29, 0x00,
 0x3F, 0x3F, 0x14, 0x00, 0x3F, 0x3F, 0x00, 0x00,
 0x3A, 0x37, 0x03, 0x00, 0x35, 0x30, 0x07, 0x00,
 0x30, 0x2A, 0x0A, 0x00, 0x2B, 0x25, 0x0D, 0x00,
 0x3F, 0x32, 0x26, 0x00, 0x3F, 0x2A, 0x19, 0x00,
 0x3F, 0x21, 0x0C, 0x00, 0x3F, 0x17, 0x00, 0x00,
 0x37, 0x16, 0x03, 0x00, 0x2F, 0x15, 0x07, 0x00,
 0x27, 0x13, 0x09, 0x00, 0x20, 0x11, 0x0A, 0x00,
 0x31, 0x3F, 0x33, 0x00, 0x2A, 0x3B, 0x2D, 0x00,
 0x25, 0x37, 0x27, 0x00, 0x1F, 0x33, 0x22, 0x00,
 0x1A, 0x2F, 0x1C, 0x00, 0x16, 0x2B, 0x18, 0x00,
 0x12, 0x27, 0x13, 0x00, 0x0E, 0x23, 0x10, 0x00,
 0x0A, 0x1F, 0x0C, 0x00, 0x08, 0x1B, 0x09, 0x00,
 0x05, 0x17, 0x06, 0x00, 0x03, 0x13, 0x04, 0x00,
 0x01, 0x0F, 0x02, 0x00, 0x00, 0x0B, 0x01, 0x00,
 0x00, 0x08, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
 0x34, 0x3F, 0x3F, 0x00, 0x2D, 0x3B, 0x3A, 0x00,
 0x27, 0x37, 0x36, 0x00, 0x22, 0x34, 0x32, 0x00,
 0x1C, 0x30, 0x2E, 0x00, 0x18, 0x2C, 0x29, 0x00,
 0x13, 0x29, 0x25, 0x00, 0x10, 0x25, 0x20, 0x00,
 0x0C, 0x21, 0x1C, 0x00, 0x09, 0x1E, 0x18, 0x00,
 0x06, 0x1A, 0x14, 0x00, 0x04, 0x16, 0x10, 0x00,
 0x02, 0x13, 0x0D, 0x00, 0x01, 0x0F, 0x09, 0x00,
 0x00, 0x0B, 0x06, 0x00, 0x00, 0x08, 0x04, 0x00,
 0x3F, 0x3F, 0x2A, 0x00, 0x3B, 0x3B, 0x25, 0x00,
 0x37, 0x37, 0x20, 0x00, 0x33, 0x34, 0x1B, 0x00,
 0x2F, 0x30, 0x17, 0x00, 0x2B, 0x2D, 0x13, 0x00,
 0x27, 0x29, 0x10, 0x00, 0x23, 0x25, 0x0D, 0x00,
 0x1F, 0x22, 0x0A, 0x00, 0x1B, 0x1E, 0x07, 0x00,
 0x18, 0x1B, 0x05, 0x00, 0x14, 0x17, 0x03, 0x00,
 0x10, 0x13, 0x01, 0x00, 0x0D, 0x10, 0x01, 0x00,
 0x0A, 0x0C, 0x00, 0x00, 0x07, 0x09, 0x00, 0x00,
 0x35, 0x3F, 0x35, 0x00, 0x27, 0x3F, 0x27, 0x00,
 0x1A, 0x3F, 0x1A, 0x00, 0x0C, 0x3F, 0x0C, 0x00,
 0x00, 0x3F, 0x00, 0x00, 0x06, 0x34, 0x07, 0x00,
 0x0B, 0x29, 0x0C, 0x00, 0x0D, 0x1F, 0x0E, 0x00,
 0x3F, 0x36, 0x3F, 0x00, 0x38, 0x29, 0x38, 0x00,
 0x31, 0x1E, 0x31, 0x00, 0x2A, 0x14, 0x2A, 0x00,
 0x24, 0x0C, 0x24, 0x00, 0x1D, 0x06, 0x1D, 0x00,
 0x16, 0x02, 0x16, 0x00, 0x10, 0x00, 0x10, 0x00,
 0x30, 0x3F, 0x3F, 0x00, 0x29, 0x3F, 0x3F, 0x00,
 0x1B, 0x3F, 0x3F, 0x00, 0x00, 0x3F, 0x3F, 0x00,
 0x01, 0x37, 0x37, 0x00, 0x04, 0x30, 0x30, 0x00,
 0x05, 0x28, 0x28, 0x00, 0x06, 0x21, 0x21, 0x00,
 0x22, 0x27, 0x3F, 0x00, 0x19, 0x1E, 0x3F, 0x00,
 0x10, 0x14, 0x3F, 0x00, 0x07, 0x0A, 0x3F, 0x00,
 0x00, 0x00, 0x3F, 0x00, 0x03, 0x03, 0x33, 0x00,
 0x05, 0x05, 0x27, 0x00, 0x06, 0x06, 0x1B, 0x00,
 0x3A, 0x3A, 0x3F, 0x00, 0x32, 0x32, 0x3B, 0x00,
 0x2C, 0x2C, 0x37, 0x00, 0x26, 0x26, 0x33, 0x00,
 0x20, 0x20, 0x30, 0x00, 0x1A, 0x1A, 0x2C, 0x00,
 0x16, 0x16, 0x28, 0x00, 0x11, 0x11, 0x24, 0x00,
 0x0D, 0x0D, 0x21, 0x00, 0x0A, 0x0A, 0x1D, 0x00,
 0x07, 0x07, 0x19, 0x00, 0x04, 0x04, 0x16, 0x00,
 0x02, 0x02, 0x12, 0x00, 0x01, 0x01, 0x0E, 0x00,
 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x07, 0x00,
 0x3E, 0x36, 0x2A, 0x00, 0x3A, 0x31, 0x24, 0x00,
 0x36, 0x2D, 0x20, 0x00, 0x32, 0x29, 0x1B, 0x00,
 0x2F, 0x25, 0x17, 0x00, 0x2B, 0x21, 0x13, 0x00,
 0x27, 0x1D, 0x0F, 0x00, 0x23, 0x1A, 0x0C, 0x00,
 0x20, 0x17, 0x09, 0x00, 0x1C, 0x13, 0x07, 0x00,
 0x18, 0x10, 0x04, 0x00, 0x15, 0x0D, 0x03, 0x00,
 0x11, 0x0A, 0x01, 0x00, 0x0D, 0x08, 0x00, 0x00,
 0x09, 0x05, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00,
 0x3F, 0x39, 0x2E, 0x00, 0x3B, 0x34, 0x28, 0x00,
 0x38, 0x2F, 0x23, 0x00, 0x35, 0x2A, 0x1E, 0x00,
 0x32, 0x25, 0x1A, 0x00, 0x2E, 0x21, 0x16, 0x00,
 0x2B, 0x1C, 0x12, 0x00, 0x28, 0x17, 0x0F, 0x00,
 0x24, 0x13, 0x0C, 0x00, 0x21, 0x0F, 0x09, 0x00,
 0x1E, 0x0B, 0x06, 0x00, 0x1B, 0x08, 0x04, 0x00,
 0x17, 0x05, 0x02, 0x00, 0x14, 0x02, 0x01, 0x00,
 0x11, 0x01, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00,
 0x36, 0x36, 0x31, 0x00, 0x30, 0x30, 0x2A, 0x00,
 0x2A, 0x2A, 0x24, 0x00, 0x24, 0x24, 0x1E, 0x00,
 0x1F, 0x1F, 0x19, 0x00, 0x19, 0x19, 0x13, 0x00,
 0x13, 0x13, 0x0E, 0x00, 0x0E, 0x0E, 0x0A, 0x00,
 0x3D, 0x37, 0x33, 0x00, 0x3A, 0x33, 0x2F, 0x00,
 0x38, 0x30, 0x2A, 0x00, 0x35, 0x2C, 0x26, 0x00,
 0x33, 0x29, 0x23, 0x00, 0x30, 0x26, 0x1F, 0x00,
 0x2E, 0x23, 0x1C, 0x00, 0x2B, 0x20, 0x19, 0x00,
 0x29, 0x1D, 0x16, 0x00, 0x26, 0x1B, 0x13, 0x00,
 0x24, 0x18, 0x10, 0x00, 0x21, 0x16, 0x0E, 0x00,
 0x1F, 0x13, 0x0C, 0x00, 0x1C, 0x11, 0x09, 0x00,
 0x1A, 0x0F, 0x08, 0x00, 0x17, 0x0D, 0x06, 0x00,
 0x15, 0x0B, 0x04, 0x00, 0x12, 0x09, 0x03, 0x00,
 0x10, 0x08, 0x02, 0x00, 0x0E, 0x06, 0x01, 0x00,
 0x0B, 0x04, 0x00, 0x00, 0x09, 0x03, 0x00, 0x00,
 0x06, 0x02, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00,
 0x3F, 0x3F, 0x3A, 0x00, 0x3F, 0x3E, 0x31, 0x00,
 0x3F, 0x3C, 0x29, 0x00, 0x3F, 0x3A, 0x20, 0x00,
 0x3F, 0x36, 0x18, 0x00, 0x3F, 0x32, 0x0F, 0x00,
 0x3F, 0x2D, 0x07, 0x00, 0x3F, 0x27, 0x00, 0x00,
 0x38, 0x1E, 0x00, 0x00, 0x31, 0x16, 0x00, 0x00,
 0x2B, 0x10, 0x00, 0x00, 0x24, 0x0A, 0x00, 0x00,
 0x1E, 0x06, 0x00, 0x00, 0x17, 0x02, 0x00, 0x00,
 0x10, 0x01, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
 0x37, 0x37, 0x3A, 0x00, 0x32, 0x32, 0x36, 0x00,
 0x2D, 0x2D, 0x32, 0x00, 0x29, 0x29, 0x2E, 0x00,
 0x25, 0x25, 0x2A, 0x00, 0x21, 0x21, 0x27, 0x00,
 0x1D, 0x1D, 0x23, 0x00, 0x19, 0x19, 0x1F, 0x00,
 0x15, 0x15, 0x1B, 0x00, 0x12, 0x12, 0x18, 0x00,
 0x0F, 0x0F, 0x14, 0x00, 0x0B, 0x0B, 0x10, 0x00,
 0x08, 0x08, 0x0C, 0x00, 0x05, 0x05, 0x08, 0x00,
 0x03, 0x03, 0x06, 0x00, 0x06, 0x1F, 0x1F, 0x00};


typedef struct {
  u4 Off;
  u4 Len;
} __attribute__ ((packed)) entry;

typedef struct {
  u1 Description[80]; // or empty (if all bytes set to 0x1A)
  u4 Magic; // #FFFF1A00 for U7 and #00001A1A for U8
  u4 NFiles;
  u4 U3; // CC for U7, 1 for U8
  u4 Size; // size of whole file
  u4 U5; // checksum?
  u4 U6; // 0
  u4 U7; // 0
  u4 U8; // 0

  u4 U9;  // 0
  u4 U10; // 0
  u4 U11; // 0
  u4 U12; // 0

  //entry Entries[NFiles];
} __attribute__ ((packed)) header;

typedef struct {
  u1 *Name;
                          //  U8  U8Gump  U8.SKF  Cru Cru2D  Pent Comp
  u4  Header;             //  6   6       2       6   6      8    11
  u1 *IdentSample;        //  ""  ""      \2\0    ""  ""     PSHP ""
  u4  Ident;              //  0   0       2       0   0      4    0
  u4  Special;            //  0   0       0       0   0      0    5
  u4  U1;                 //  4   4       0       4   4      0    4
  u4  NFrames;            //  2   2       0       2   2      4    2

  u4  FrameHeader;        //  6   6       0       8   8      8    6
  u4  FrameOff;           //  3   3       0       3   3      4    4
  u4  U2;                 //  1   2       0       2   2      0    0
  u4  FrameLen;           //  2   2       0       3   3      4    2
  u4  FrameLenAdd;        //  0   8       0       0   0      0    0

  u4  FrameHeader2;       //  18  18      10     28   20     20   10
  u4  U3;                 //  8   8       0      8    0      0    0
  u4  RLE;                //  2   2       2      4    4      4    2
  u4  Width;              //  2   2       2      4    4      4    2
  u4  Height;             //  2   2       2      4    4      4    2
  u4  X;                  //  2   2       2      4    4      4    2
  u4  Y;                  //  2   2       2      4    4      4    2

  u4  LineOff;            //  2   2       2      4    4      4    0
  u4  LineOffAbsolute;    //  0   0       0      0    0      1    0
} __attribute__ ((packed)) fmt;


// Shape format configuration for Crusader
static fmt Crusader = {
  "Crusader",
  6,    // header
  "",   // ident
  0,    // Ident 
  0,    // Special
  4,    // header_unk
  2,    // NFrames

  8,    // frameheader
  3,    // Off
  1,    // frameheader_unk
  4,    // Len
  0,    // Len_kludge

  28,   // frameheader2        20 for mouse/gumps
  8,    // frame_unknown       0  for mouse/gumps
  4,    // frame_compression
  4,    // frame_width
  4,    // frame_height
  4,    // frame_xoff
  4,    // frame_yoff

  4,    // LineOff
  0     // LineOffAbsolute
};

// Shape format configuration for 2D Crusader Shapes
static fmt Crusader_2D = {
  "Crusader_2D",
  6,    // header
  "",   // ident
  0,    // Ident 
  0,    // Special
  4,    // header_unk
  2,    // NFrames

  8,    // frameheader
  3,    // Off
  1,    // frameheader_unk
  4,    // Len
  0,    // Len_kludge

  20,   // frameheader2        20 for mouse/gumps
  0,    // frame_unknown       0  for mouse/gumps
  4,    // frame_compression
  4,    // frame_width
  4,    // frame_height
  4,    // frame_xoff
  4,    // frame_yoff

  4,    // LineOff
  0     // LineOffAbsolute
};


// Shape format configuration for Ultima8
static fmt Ultima8 = {
  "Ultima8",
  6,    // header
  "",   // ident
  0,    // Ident 
  0,    // Special
  4,    // header_unk
  2,    // NFrames

  6,    // frameheader
  3,    // Off
  1,    // frameheader_unk
  2,    // Len
  0,    // Len_kludge

  18,   // frameheader2
  8,    // frame_unknown
  2,    // frame_compression
  2,    // frame_width
  2,    // frame_height
  2,    // frame_xoff
  2,    // frame_yoff

  2,    // LineOff
  0     // LineOffAbsolute
};

// Shape format configuration for Ultima8 2D interface components
static fmt Ultima8_2D = {
  "Ultima8_2D",
  6,    // header
  "",   // ident
  0,    // Ident 
  0,    // Special
  4,    // header_unk
  2,    // NFrames

  6,    // frameheader
  3,    // Off
  1,    // frameheader_unk
  2,    // Len
  8,    // Len_kludge

  18,   // frameheader2
  8,    // frame_unknown
  2,    // frame_compression
  2,    // frame_width
  2,    // frame_height
  2,    // frame_xoff
  2,    // frame_yoff

  2,    // LineOff
  0     // LineOffAbsolute
};

// Shape format configuration for Ultima8 SKF
static fmt Ultima8_SKF = {
  "Ultima8_SKF",
  2,    // header
  "\2",  // ident
  2,    // Ident 
  0,    // Special
  0,    // header_unk
  0,    // NFrames

  0,    // frameheader
  0,    // Off
  0,    // frameheader_unk
  0,    // Len
  0,    // Len_kludge

  10,   // frameheader2
  0,    // frame_unknown
  2,    // frame_compression
  2,    // frame_width
  2,    // frame_height
  2,    // frame_xoff
  2,    // frame_yoff

  2,    // LineOff
  0     // LineOffAbsolute
};

// Shape format configuration for Compressed Ultima 8 shapes
static fmt Ultima8_CMP = {
  "Ultima8_CMP",
  11,   // header
  "",   // ident
  0,    // Ident 
  5,    // Special
  4,    // header_unk
  2,    // NFrames

  6,    // frameheader
  4,    // Off
  2,    // frameheader_unk
  0,    // Len
  0,    // Len_kludge

  10,   // frameheader2
  0,    // frame_unknown
  2,    // frame_compression
  2,    // frame_width
  2,    // frame_height
  2,    // frame_xoff
  2,    // frame_yoff

  0,    // LineOff
  0     // LineOffAbsolute
};


#define read1() (Q < Z+L ? *Q++ : (Q++, 0x3F))

#define readX(O, N) \
  do {if (Q+N > Z+L) goto next_fmt; \
      else if (N==1) {O = *(u1*)Q; Q+=N;} \
      else if (N==2) {O = *(u2*)Q; Q+=N;} \
      else if (N==3) {O = Q[0]|(Q[1]<<8)|(Q[2]<<16); Q+=N;} \
      else if (N==4) {O = *(u4*)Q; Q+=N;} \
      else Q+=N;} while (0)

#define readXS(O, N) \
  do {if (Q+N > Z+L) goto next_fmt; \
      else if (N==1) {O = *(s1*)Q; Q+=N;} \
      else if (N==2) {O = *(s2*)Q; Q+=N;} \
      else if (N==3) {O = Q[0]|(Q[1]<<8)|(Q[2]<<16); Q+=N;} \
      else if (N==4) {O = *(s4*)Q; Q+=N;} \
      else Q+=N;} while (0)

static int calcNumFrames(u1 *Z, u1 *Q, fmt *Fmt, int L) {
  int f=0, Off, Len;
  u4 FirstOff = 0xFFFFFFFF;

  for (f=0;;f++) {
    Q = Z + Fmt->Header + f*Fmt->FrameHeader;

    if ((Q-Z) >= FirstOff) break;

    Off = Fmt->Header + f*Fmt->FrameHeader;
    if (Fmt->FrameOff) {
      readX(Off, Fmt->FrameOff);
      Off += Fmt->Special;
    }

    if (Off < FirstOff) FirstOff = Off;

    Q += Fmt->U2; // skip unknown
    Len = L-Off; // frame length
    if (Fmt->FrameLen) {
      readX(Len, Fmt->FrameLen);
      Len += Fmt->FrameLenAdd;
    }
  }
next_fmt:
  return f;
}

static fmt *Fmts[] = {
  &Ultima8, &Ultima8_2D, &Ultima8_SKF, &Crusader, &Crusader_2D, &Ultima8_CMP, 0
};

static int saveGfx(char *Output, u1 *Z, int L) {
  pic *P = 0;
  char Tmp[1024], Tmp2[64];
  int I, J, K=0, C, X, Y, LineOff, DLen, HighestOff, Run;
  int ColorKey=0xFF, NFrames, Off, Len, XO, YO, W, H, RLELen, RLE;
  u1 *Q, *R;
  fmt *Fmt;

next_fmt:
  Q = Z;
  NFrames=1;
  if (P) {picDel(P); P = 0;}
  unless (Fmt = Fmts[K++]) {
    printf("    Cant infer sprite format\n");
    return 0;
  }

  if (Q+Fmt->Ident > Z+L || memcmp(Q,Fmt->IdentSample,Fmt->Ident)) return 0;

  Q += Fmt->Ident;
  Q += Fmt->Special; // header special colour
  Q += Fmt->U1; // unknown

  readX(NFrames, Fmt->NFrames);
  unless (NFrames) NFrames = calcNumFrames(Z, Q, Fmt, L);
  unless (NFrames) goto next_fmt;
  times (I, NFrames) {
    Q = Z + Fmt->Header + I*Fmt->FrameHeader;

    Off = Fmt->Header + I*Fmt->FrameHeader;
    if (Fmt->FrameOff) {
      readX(Off, Fmt->FrameOff);
      Off += Fmt->Special;
    }

    Q += Fmt->U2; // skip unknown

    Len = L-Off;
    if (Fmt->FrameLen) {
      readX(Len, Fmt->FrameLen);
      Len += Fmt->FrameLenAdd;
    }
    if ((Len + Off) > L) goto next_fmt;

    Q = Z + Off; // start of frame
    Q += Fmt->U3; // skip unknown

    readX(RLE, Fmt->RLE);
    readXS(W, Fmt->Width);
    readXS(H, Fmt->Height);
    readXS(XO, Fmt->X);
    readXS(YO, Fmt->Y);

    if ((RLE != 0 && RLE != 1) || W <= 0 || H <= 0 || W>1024 || H>1024)
      goto next_fmt;

    HighestOff = 0;

    RLELen = Len - (Fmt->FrameHeader2+(H*Fmt->LineOff));
    if (RLELen < 0) goto next_fmt;
    if (Fmt->Special) {
      printf("    Cant handle u8shapes.cmp: find u8shapes.flx\n");
      return 0;
    }
    Q = Z + Off + Fmt->FrameHeader2; // scanline offset table


    times (Y, H) { // find offset of the last line
      readX(LineOff, Fmt->LineOff);
      if (!Fmt->LineOffAbsolute) LineOff -= (H-Y)*Fmt->LineOff;
      if (LineOff > RLELen) goto next_fmt;
      if (LineOff > HighestOff) HighestOff = LineOff;
    }

    // now calculate length of last line
    Q = Z + Off + Fmt->FrameHeader2 + H*Fmt->LineOff + HighestOff;

    X = DLen = 0;
    if (RLE) {
      do {
        X += read1();
        if (X == W) break;
        DLen = read1();
        Q += (DLen&1) ? 1 : (DLen>>1);
        X += (DLen>>1);
      } while (X < W);
    } else {
      do {
        X += read1();
        if (X == W) break;
        DLen = read1();
        Q += DLen;
        X += DLen;
      } while (X < W);
    }

    if (RLELen < (Q - Z - Off - Fmt->FrameHeader2 - H*Fmt->LineOff))
      goto next_fmt;
  }

  printf("    NFrames=%d Format=%s\n", NFrames, Fmt->Name);

  times (I, NFrames) {
    Q = Z + Fmt->Header + I*Fmt->FrameHeader;

    Off = Fmt->Header + I*Fmt->FrameHeader;
    if (Fmt->FrameOff) {
      readX(Off, Fmt->FrameOff);
      Off += Fmt->Special;
    }

    Q += Fmt->U2; // skip unknown

    Len = L-Off;
    if (Fmt->FrameLen) {
      readX(Len, Fmt->FrameLen);
      Len += Fmt->FrameLenAdd;
    }
    Q = Z + Off; // start of frame
    Q += Fmt->U3; // skip unknown

    readX(RLE, Fmt->RLE);
    readXS(W, Fmt->Width);
    readXS(H, Fmt->Height);
    readXS(XO, Fmt->X);
    readXS(YO, Fmt->Y);
    if (W<=1 && H<=1) continue; // skip dummy images

    //printf("      RLE=%d X=%d:%d Y=%d:%d\n", RLE, XO, W, YO, H);

    P = picNew(W, H, 8);
    picClear(P, ColorKey);
    memcpy(P->P, U8_Pal, 256*4);
    times (J, 256*4) P->P[J]<<=2;
    P->P[ColorKey*4+0] = 0;
    P->P[ColorKey*4+1] = 0xFF;
    P->P[ColorKey*4+2] = 0xFF;

    times (Y, H) {
      Q = Z + Off + Fmt->FrameHeader2 + Y*Fmt->LineOff;
      readX(LineOff, Fmt->LineOff);
      unless (Fmt->LineOffAbsolute) LineOff -= (H-Y)*Fmt->LineOff;
      Q = Z + Off + Fmt->FrameHeader2 + H*Fmt->LineOff + LineOff;

      for (X=0; X<W; ) {
        X += read1();
        if (X >= W) break;
        C = read1();
        if (!RLE) Run = 0;
        else {Run = C&1; C >>= 1;}
        if (Run) {
          J = read1();
          while (C--) picPut(P, X++, Y, J);
        } else {
          while (C--) picPut(P, X++, Y, read1());
        }
      }
    }
    sprintf(Tmp, "%s/%04d.png", Output, I);
    pngSave(Tmp, P);

    sprintf(Tmp2, "(%d %d)", XO, YO);
    sprintf(Tmp, "%s/%04d.txt", Output, I);
    writeFile(0, strlen(Tmp2), Tmp, Tmp2);

    picDel(P);
  }
}


static int saveGfxU7(char *Output, u1 *M, int L) {
  char Tmp[1024];
  int I, J, C, W, H, X,Y,EX, MinX=0, MinY=0, MaxX=0, MaxY=0, *Usage;
  s2 X0,Y0,X1,Y1;
  u1 *Q;
  pic *P;

  if (*(u4*)M == L) {
    u4 *O = (u4*)M+1;
    int NFrames = *O/4-1;
    printf("  NFrames=%d\n", NFrames);
    times (I, NFrames) {
      Q = M+O[I]+8;
      Usage = ns(u4,256);
      while ((C=ru2(Q))) {
        X = (s2)ru2(Q);
        Y = (s2)ru2(Q);
        EX = X+(C>>1);
        MinX = min(MinX,X);
        MaxX = max(MaxX,EX);
        MinY = min(MinY,Y);
        MaxY = max(MaxY,Y);

        unless (C&1) while (X<EX) X++,Usage[*Q++]++;
        else while (X<EX) {
          C = *Q++;
          if (C&1) { //run
            C>>=1;
            while (C--) X++,Usage[*Q]++;
            Q++;
          } else { //copy
            C>>=1;
            while (C--) X++,Usage[*Q++]++;
          }
        }
      }

      Q = M+O[I];
      X0 = ru2(Q);
      X1 = ru2(Q);
      Y1 = ru2(Q);
      Y0 = ru2(Q);

      W=MaxX-MinX;
      H=MaxY-MinY;
      if (W<1) W=1;
      if (H<1) H=1;

      //printf("  %03d,%x: %dx%d %d,%d+%d,%d\n", I, O[I], W,H, X0,Y0, X1,Y1);

      P = picNew(W,H,8);

      times (J, 256) {
        P->P[J*4+0] = U7_Pal[J*3+0];
        P->P[J*4+1] = U7_Pal[J*3+1];
        P->P[J*4+2] = U7_Pal[J*3+2];
      }

      P->K=255;
      times (J, 256) if (!Usage[J]) {
        P->K = J;
        break;
      }
      memset(P->D,P->K,P->W*P->H);

      MinX = MinX<0?-MinX:0;
      MinY = MinY<0?-MinY:0;
      while ((C=ru2(Q))) {
        X = (s2)ru2(Q);
        Y = (s2)ru2(Q);
        EX = X+(C>>1);
        unless (C&1) while (X<EX) picPut(P,MinX+X++,MinY+Y,*Q++);
        else while (X<EX) {
          C = *Q++;
          if (C&1) { //run
            C>>=1;
            while (C--) picPut(P,MinX+X++,MinY+Y,*Q);
            Q++;
          } else { //copy
            C>>=1;
            while (C--) picPut(P,MinX+X++,MinY+Y,*Q++);
          }
        }
      }

      sprintf(Tmp, "%s/%04d.png", Output, I);
      pngSave(Tmp, P);
    }
  } else { // 8x8 tileset
    P = picNew(8,L/8,8);
    times (J, 256) {
      P->P[J*4+0] = U7_Pal[J*3+0];
      P->P[J*4+1] = U7_Pal[J*3+1];
      P->P[J*4+2] = U7_Pal[J*3+2];
    }
    memcpy(P->D,M,L);
    sprintf(Tmp, "%s.png", Output);
    pngSave(Tmp, P);
  }
}

static void flxDecompile(char *Output, char *Input) {
  char *Name, FLXName[256], Tmp[1024], Tmp2[256], Dir[64], File[64],Ext[32];
  int I, L = fileSize(Input);
  u1 *Q, *M = readFile(0, L, Input);
  header *H = (header*)M;
  entry *E = (entry*)(H+1);

  initU7Shapes();

  pathParts(0, FLXName, Ext, Input);
  downcase(FLXName);
  downcase(Ext);
  printf("NFiles=%d\n", H->NFiles);
  times (I, H->NFiles) {
    unless (E[I].Off) continue;
    if (I != 844)
    if (!strcmp(FLXName,"u8shapes")) Name = u8shapes[I];
    else Name = 0;
    if (Name) {
      if (!strcmp(Name, "Dummy")) continue;
      pathParts(Dir, File, 0, Name);
      if (isdigit(File[0])) {
        sprintf(Tmp2, "%s/%04d", Dir, I);
        Name = Tmp2;
      }
    } else if (!strcmp(FLXName,"shapes") && !strcmp(Ext,"vga")) {
      if (memcmp(H->Description, "Serpent", 7)) sprintf(Tmp2,"%s_%04d", U71Shapes[I], I);
      else sprintf(Tmp2,"%s_%04d", U72Shapes[I], I);
      Name = Tmp2;
    } else {
      sprintf(Tmp2, "%04d", I);
      Name = Tmp2;
    }
    Q = M + E[I].Off;
    printf("Extracting: %s\n", Name);
    printf("  %04d: Off=#%08X Len=#%08X\n", I, E[I].Off, E[I].Len);
    //writeFile(0, E[I].Len, "out/1.bin", Q);
    sprintf(Tmp, "%s/%s", Output, Name);
    if (H->Magic == 0xFFFF1A00) saveGfxU7(Tmp, Q, E[I].Len);
    else unless(saveGfx(Tmp, Q, E[I].Len)) {
      writeFile(0, E[I].Len, Tmp, Q);
    }
  }
}

int flxInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "flx";
  F->Description = "Ultima 7, 8 and Crusader No Remorse archives";
  F->Decompile = flxDecompile;
  return 1;
}

