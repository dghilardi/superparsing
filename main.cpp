#include "retrimage.h"
#include "queryimage.h"
#include "retrievalset.h"
#include "neighbourstat.h"
#include <getopt.h>

#include "opencv2/highgui/highgui.hpp"

#define IMG_NAME "street_art1030"

using namespace std;

void help(string progName){
    cout << "usage: " << progName << " [ options ]" << endl;
    cout << "  -q <file>    --query <file>      Query image to label" << endl;
    cout << "  -r           --retrain           Redo the training phase" << endl;
    cout << "  -h           --help              Gives this help" << endl;
}

int main(int argc, char **argv){
    vector<string> retrievalSet;
    string queryPath;
    static struct option long_options[] = {
        {"query", 1, 0, 'q'},
        {"retrain", 1, 0, 'r'},
        {"help", 0, 0, 'h'},
        {NULL, 0, NULL, 0}
    };
    if(argc!=1){
        int c, option_index=0;
        while((c=getopt_long(argc, argv, "q:rh", long_options, &option_index))!=-1){
            int this_option_optind = optind ? optind : 1;
            cout << "arg: " << c << endl;
            switch(c){
            case 'q':
                queryPath = optarg;
                break;
            case 'r':
                //TODO
                break;
            case 'h':
                help(argv[0]);
                return 0;
                break;
            case 0:
                retrievalSet.push_back(optarg);
                break;
            default:
                help(argv[0]);
                return 1;
                break;
            }
        }
    }else{
        queryPath = IMG_NAME;
    }
        /*retrievalSet.push_back("street_a205062");
        retrievalSet.push_back("street_a232022");
        retrievalSet.push_back("street_a281067");
        retrievalSet.push_back("street_a714064");
        retrievalSet.push_back("street_a79094");
        retrievalSet.push_back("street_a805038");
        retrievalSet.push_back("street_art1030");
        retrievalSet.push_back("street_art1041");
        retrievalSet.push_back("street_art1162");
        retrievalSet.push_back("street_art1187");
        retrievalSet.push_back("street_art1199");*/
        retrievalSet.push_back("street_art1206");
        retrievalSet.push_back("street_art1328");
        retrievalSet.push_back("street_art1583");
        retrievalSet.push_back("street_art1686");
        retrievalSet.push_back("street_art1688");
        retrievalSet.push_back("street_art256");
        retrievalSet.push_back("street_art379");
        retrievalSet.push_back("street_art475");
        retrievalSet.push_back("street_art511");
        retrievalSet.push_back("street_art546");
        retrievalSet.push_back("street_art549");
        retrievalSet.push_back("street_art757");
        retrievalSet.push_back("street_art758");
        retrievalSet.push_back("street_art759");
        retrievalSet.push_back("street_art760");
        retrievalSet.push_back("street_art761");
        retrievalSet.push_back("street_art762");
        retrievalSet.push_back("street_art764");
        retrievalSet.push_back("street_art766");
        retrievalSet.push_back("street_art767");
        retrievalSet.push_back("street_art768");
        retrievalSet.push_back("street_art771");
        retrievalSet.push_back("street_art775");
        retrievalSet.push_back("street_art779");
        retrievalSet.push_back("street_art793");
        retrievalSet.push_back("street_art798");
        retrievalSet.push_back("street_art799");
        retrievalSet.push_back("street_art838");
        retrievalSet.push_back("street_art861");
        /*retrievalSet.push_back("street_art862");
        retrievalSet.push_back("street_art869");
        retrievalSet.push_back("street_art880");
        retrievalSet.push_back("street_art923");
        retrievalSet.push_back("street_art970");
        retrievalSet.push_back("street_art976");
        retrievalSet.push_back("street_artc14");
        retrievalSet.push_back("street_artc27");
        retrievalSet.push_back("street_bost104");
        retrievalSet.push_back("street_bost105");
        retrievalSet.push_back("street_bost111");
        retrievalSet.push_back("street_bost121");
        retrievalSet.push_back("street_bost136");
        retrievalSet.push_back("street_bost137");
        retrievalSet.push_back("street_bost26");
        retrievalSet.push_back("street_bost40");
        retrievalSet.push_back("street_bost45");
        retrievalSet.push_back("street_bost46");
        retrievalSet.push_back("street_bost50");
        retrievalSet.push_back("street_bost56");
        retrievalSet.push_back("street_bost60");
        retrievalSet.push_back("street_bost64");
        retrievalSet.push_back("street_bost65");
        retrievalSet.push_back("street_bost72");
        retrievalSet.push_back("street_bost75");
        retrievalSet.push_back("street_bost76");
        retrievalSet.push_back("street_bost77");
        retrievalSet.push_back("street_bost81");
        retrievalSet.push_back("street_bost82");
        retrievalSet.push_back("street_bost89");
        retrievalSet.push_back("street_boston18");
        retrievalSet.push_back("street_boston19");
        retrievalSet.push_back("street_boston1");
        retrievalSet.push_back("street_boston21");
        retrievalSet.push_back("street_boston235");
        retrievalSet.push_back("street_boston241");
        retrievalSet.push_back("street_boston255");
        retrievalSet.push_back("street_boston263");
        retrievalSet.push_back("street_boston270");
        retrievalSet.push_back("street_boston271");
        retrievalSet.push_back("street_boston272");
        retrievalSet.push_back("street_boston274");
        retrievalSet.push_back("street_boston286");
        retrievalSet.push_back("street_boston289");
        retrievalSet.push_back("street_boston306");
        retrievalSet.push_back("street_boston32");
        retrievalSet.push_back("street_boston339");
        retrievalSet.push_back("street_boston345");
        retrievalSet.push_back("street_boston351");
        retrievalSet.push_back("street_boston353");
        retrievalSet.push_back("street_boston356");
        retrievalSet.push_back("street_boston360");
        retrievalSet.push_back("street_boston373");
        retrievalSet.push_back("street_boston378");
        retrievalSet.push_back("street_boston379");
        retrievalSet.push_back("street_boston396");
        retrievalSet.push_back("street_boston397");
        retrievalSet.push_back("street_boston408");
        retrievalSet.push_back("street_boston46");
        retrievalSet.push_back("street_boston61");
        retrievalSet.push_back("street_boston68");
        retrievalSet.push_back("street_boston79");
        retrievalSet.push_back("street_city42");
        retrievalSet.push_back("street_city91");
        retrievalSet.push_back("street_enc15");
        retrievalSet.push_back("street_enc48");
        retrievalSet.push_back("street_gre114");
        retrievalSet.push_back("street_gre115");
        retrievalSet.push_back("street_gre116");
        retrievalSet.push_back("street_gre11");
        retrievalSet.push_back("street_gre121");
        retrievalSet.push_back("street_gre122");
        retrievalSet.push_back("street_gre124");
        retrievalSet.push_back("street_gre126");
        retrievalSet.push_back("street_gre128");
        retrievalSet.push_back("street_gre130");
        retrievalSet.push_back("street_gre131");
        retrievalSet.push_back("street_gre135");
        retrievalSet.push_back("street_gre159");
        retrievalSet.push_back("street_gre160");
        retrievalSet.push_back("street_gre179");
        retrievalSet.push_back("street_gre181");
        retrievalSet.push_back("street_gre186");
        retrievalSet.push_back("street_gre189");
        retrievalSet.push_back("street_gre191");
        retrievalSet.push_back("street_gre193");
        retrievalSet.push_back("street_gre209");
        retrievalSet.push_back("street_gre211");
        retrievalSet.push_back("street_gre214");
        retrievalSet.push_back("street_gre219");
        retrievalSet.push_back("street_gre251");
        retrievalSet.push_back("street_gre27");
        retrievalSet.push_back("street_gre295");
        retrievalSet.push_back("street_gre2");
        retrievalSet.push_back("street_gre600");
        retrievalSet.push_back("street_gre653");
        retrievalSet.push_back("street_gre76");
        retrievalSet.push_back("street_gre86");
        retrievalSet.push_back("street_gre91");
        retrievalSet.push_back("street_gre95");
        retrievalSet.push_back("street_hexp12");
        retrievalSet.push_back("street_hexp17");
        retrievalSet.push_back("street_hexp21");
        retrievalSet.push_back("street_hexp22");
        retrievalSet.push_back("street_hexp23");
        retrievalSet.push_back("street_hexp28");
        retrievalSet.push_back("street_hexp29");
        retrievalSet.push_back("street_hexp30");
        retrievalSet.push_back("street_hexp3");
        retrievalSet.push_back("street_hexp4");
        retrievalSet.push_back("street_land767");
        retrievalSet.push_back("street_par100");
        retrievalSet.push_back("street_par101");
        retrievalSet.push_back("street_par102");
        retrievalSet.push_back("street_par103");
        retrievalSet.push_back("street_par107");
        retrievalSet.push_back("street_par112");
        retrievalSet.push_back("street_par113");
        retrievalSet.push_back("street_par116");
        retrievalSet.push_back("street_par118");
        retrievalSet.push_back("street_par119");
        retrievalSet.push_back("street_par120");
        retrievalSet.push_back("street_par123");
        retrievalSet.push_back("street_par124");
        retrievalSet.push_back("street_par125");
        retrievalSet.push_back("street_par12");
        retrievalSet.push_back("street_par134");
        retrievalSet.push_back("street_par140");
        retrievalSet.push_back("street_par141");
        retrievalSet.push_back("street_par143");
        retrievalSet.push_back("street_par146");
        retrievalSet.push_back("street_par14");
        retrievalSet.push_back("street_par150");
        retrievalSet.push_back("street_par151");
        retrievalSet.push_back("street_par152");
        retrievalSet.push_back("street_par153");
        retrievalSet.push_back("street_par154");
        retrievalSet.push_back("street_par155");
        retrievalSet.push_back("street_par156");
        retrievalSet.push_back("street_par158");
        retrievalSet.push_back("street_par160");
        retrievalSet.push_back("street_par161");
        retrievalSet.push_back("street_par162");
        retrievalSet.push_back("street_par165");
        retrievalSet.push_back("street_par169");
        retrievalSet.push_back("street_par16");
        retrievalSet.push_back("street_par177");
        retrievalSet.push_back("street_par178");
        retrievalSet.push_back("street_par185");
        retrievalSet.push_back("street_par188");
        retrievalSet.push_back("street_par18");
        retrievalSet.push_back("street_par192");
        retrievalSet.push_back("street_par197");
        retrievalSet.push_back("street_par198");
        retrievalSet.push_back("street_par199");
        retrievalSet.push_back("street_par1");
        retrievalSet.push_back("street_par203");
        retrievalSet.push_back("street_par20");
        retrievalSet.push_back("street_par21");
        retrievalSet.push_back("street_par22");
        retrievalSet.push_back("street_par28");
        retrievalSet.push_back("street_par29");
        retrievalSet.push_back("street_par33");
        retrievalSet.push_back("street_par36");
        retrievalSet.push_back("street_par3");
        retrievalSet.push_back("street_par42");
        retrievalSet.push_back("street_par49");
        retrievalSet.push_back("street_par4");
        retrievalSet.push_back("street_par52");
        retrievalSet.push_back("street_par54");
        retrievalSet.push_back("street_par59");
        retrievalSet.push_back("street_par5");
        retrievalSet.push_back("street_par64");
        retrievalSet.push_back("street_par68");
        retrievalSet.push_back("street_par69");
        retrievalSet.push_back("street_par6");
        retrievalSet.push_back("street_par70");
        retrievalSet.push_back("street_par72");
        retrievalSet.push_back("street_par74");
        retrievalSet.push_back("street_par76");
        retrievalSet.push_back("street_par77");
        retrievalSet.push_back("street_par78");
        retrievalSet.push_back("street_par79");
        retrievalSet.push_back("street_par80");
        retrievalSet.push_back("street_par81");
        retrievalSet.push_back("street_par83");
        retrievalSet.push_back("street_par84");
        retrievalSet.push_back("street_par85");
        retrievalSet.push_back("street_par86");
        retrievalSet.push_back("street_par87");
        retrievalSet.push_back("street_par88");
        retrievalSet.push_back("street_par90");
        retrievalSet.push_back("street_par91");
        retrievalSet.push_back("street_par93");
        retrievalSet.push_back("street_par94");
        retrievalSet.push_back("street_par95");
        retrievalSet.push_back("street_par97");
        retrievalSet.push_back("street_par99");
        retrievalSet.push_back("street_par9");
        retrievalSet.push_back("street_street110");
        retrievalSet.push_back("street_street22");
        retrievalSet.push_back("street_street27");
        retrievalSet.push_back("street_street3");
        retrievalSet.push_back("street_street47");
        retrievalSet.push_back("street_street48");
        retrievalSet.push_back("street_street50");
        retrievalSet.push_back("street_street56");
        retrievalSet.push_back("street_street59");
        retrievalSet.push_back("street_street61");
        retrievalSet.push_back("street_street62");
        retrievalSet.push_back("street_street85");
        retrievalSet.push_back("street_street94");
        retrievalSet.push_back("street_street98");
        retrievalSet.push_back("street_urb104");
        retrievalSet.push_back("street_urb19");
        retrievalSet.push_back("street_urb200");
        retrievalSet.push_back("street_urb206");
        retrievalSet.push_back("street_urb228");
        retrievalSet.push_back("street_urb244");
        retrievalSet.push_back("street_urb265");
        retrievalSet.push_back("street_urb274");
        retrievalSet.push_back("street_urb294");
        retrievalSet.push_back("street_urb304");
        retrievalSet.push_back("street_urb305");
        retrievalSet.push_back("street_urb332");
        retrievalSet.push_back("street_urb382");
        retrievalSet.push_back("street_urb506");
        retrievalSet.push_back("street_urb510");
        retrievalSet.push_back("street_urb521");
        retrievalSet.push_back("street_urb562");
        retrievalSet.push_back("street_urb589");
        retrievalSet.push_back("street_urb661");
        retrievalSet.push_back("street_urb693");
        retrievalSet.push_back("street_urb722");
        retrievalSet.push_back("street_urb761");
        retrievalSet.push_back("street_urb798");
        retrievalSet.push_back("street_urb805");
        retrievalSet.push_back("street_urb830");
        retrievalSet.push_back("street_urb834");
        retrievalSet.push_back("street_urb837");
        retrievalSet.push_back("street_urb848");
        retrievalSet.push_back("street_urb866");
        retrievalSet.push_back("street_urb885");
        retrievalSet.push_back("street_urb927");
        retrievalSet.push_back("street_urb928");
        retrievalSet.push_back("street_urb983");
        retrievalSet.push_back("street_urban950");
        retrievalSet.push_back("street_urban951");
        retrievalSet.push_back("street_urban954");
        retrievalSet.push_back("street_urban976");
        retrievalSet.push_back("street_urban996");
        retrievalSet.push_back("street_urban997");*/
    QueryImage queryImg(queryPath);
    RetrievalSet db(retrievalSet);
    db.LabelImg(queryImg);
    cv::waitKey();
    return 0;
}

