#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <allegro.h>

int hipotenus(int,int,int,int);
int uzaklik(int,int);
bool oyunaBasla(void);
void ayarlar(void);
void yuksekskor(void);

void main() {

	allegro_init();
	install_keyboard();
	install_mouse();
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,"A");
	set_color_depth(32);
	set_window_title("Tank Vurma Oyunu");
	set_gfx_mode( GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	show_mouse(screen);

	BITMAP *ekran;
	ekran=create_bitmap(800,600);
	clear_to_color(ekran,makecol( 64, 0, 64) );
	textprintf_centre_ex(ekran,font,400,100,makecol(255, 255, 255),-1, "BASLA");
	textprintf_centre_ex(ekran,font,400,200,makecol(255, 255, 255),-1, "AYARLAR");
	textprintf_centre_ex(ekran,font,400,300,makecol(255, 255, 255),-1, "YUKSEK SKOR");
	textprintf_centre_ex(ekran,font,400,400,makecol(255, 255, 255),-1, "CIKIS");
	
	rect(ekran,300,60,500,140,makecol(255,255,255));
	rect(ekran,300,160,500,240,makecol(255,255,255));
	rect(ekran,300,260,500,340,makecol(255,255,255));
	rect(ekran,300,360,500,440,makecol(255,255,255));
	bool returnOyunaBasla=false; //Bu deðiþken ile yenile butonuna basýlma iþlemini kontrol ediyoruz.
	//Yenile butonuna basýlmýþsa oyunaBasla fonksiyonu geriye true deðerini döndürüyor ve çalýþmakta olan fonksiyon sonlandýrýlýyor.
	//returnOyunaBasla deðiþkeni true olduðu zaman direk olarak yeni bir oyunaBasla fonksiyonu çaðýrýyoruz.
	//Eðer bu deðiþkeni kullanmadan oyunaBasla fonksiyonu içinde yenile butonuna basýldýðýnda direk yeni bir oyunaBasla fonksiyonu çaðýrýrsak
	//o an çalýþmakta olan oyunaBasla fonksiyonunu bitirmeden yeni bir oyunaBasla fonksiyonu daha oluþturmuþ olacaktýk. Bu þekilde sürekli iç içe
	//fonksiyonlar oluþturmak hafýza sorununa sebep olacaktý.
	while(1){
		if(returnOyunaBasla==true){
			returnOyunaBasla=oyunaBasla();
		}
		blit(ekran,screen,0,0,0,0,800,600);
		if(mouse_x>300 && mouse_y>60 && mouse_x<500 && mouse_y<140 && mouse_b==1){
			returnOyunaBasla=oyunaBasla();
		}
		if(mouse_x>300 && mouse_y>160 && mouse_x<500 && mouse_y<240 && mouse_b==1){
			ayarlar();
		}
		if(mouse_x>300 && mouse_y>260 && mouse_x<500 && mouse_y<340 && mouse_b==1){
			yuksekskor();
		}
		if(mouse_x>300 && mouse_y>360 && mouse_x<500 && mouse_y<440 && mouse_b==1){
			exit(0);
		}
		rest(100);
	}
}
END_OF_MAIN();

int hipotenus(int x1,int x2,int y1,int y2){

	int sonuc,x;
	double dsonuc,dx;
	x=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
	dx=(double)x;
	dsonuc=sqrt(dx);
	sonuc=(int)dsonuc;
	return sonuc;
}

int uzaklik(int a,int b){

	int sonuc;
	sonuc=abs(a-b);
	return sonuc;
}

bool oyunaBasla(void){
	
	BITMAP *ekran,*menu,*yenile,*cikis;
	SAMPLE *bang=load_sample("bang.wav");
	int binayuksekligi[8],x,y,renkr,renkg,renkb,konum1,konum2,oyuncu1x,oyuncu1y,oyuncu2x,oyuncu2y,topx,topy,m_x,m_y,v0,ruzgar,siradakix,siradakiy,karsidakix,karsidakiy,hak1,hak2,r1,g1,b1,r2,g2,b2,skor1,skor2,s1,s2,s3,zaman;
	float t,sina,cosa;
	double rsin,rcos,ruzgaryonu,radyanruzgaryonu;
	bool oyuncudegistir;
	srand(time(NULL));

	//player 1'in sol 3 kuleden hangisinin üzerinde olacaðý random olarak belirleniyor.
	konum1=rand()%3;
	//player 2'nin sað 3 kuleden hangisinin üzerinde olacaðý random olarak belirleniyor.
	konum2=rand()%3+5;

	//menu, yenile ve çýkýþ için butonlar ekrana yazdýrýlýyor.
	ekran=create_bitmap(800,600);
	clear_to_color(ekran,makecol(127,127,127));
	blit(ekran,screen,0,0,0,0,800,600);
	menu=load_bitmap("menu.bmp",NULL);
	blit(menu,ekran,0,0,350,0,800,600);
	yenile=load_bitmap("yenile.bmp",NULL);
	blit(yenile,ekran,0,0,350,50,800,600);
	cikis=load_bitmap("cikis.bmp",NULL);
	blit(cikis,ekran,0,0,350,100,800,600);

	//8 adet kule random renklerde ve random yüksekliklerde oluþturuluyor.
	//playerlarýn üzerinde olduðu kuleye göre koordinatlarý belirleniyor.
	for(x=0;x<=7;x++){
		binayuksekligi[x]=rand()%300+300;
		renkr=rand()%255;
		renkg=rand()%255;
		renkb=rand()%255;
		rectfill(ekran,10+x*100,binayuksekligi[x],90+x*100,600,makecol(renkr,renkg,renkb));
		if(konum1==x){
			oyuncu1x=10+x*100;
			oyuncu1y=binayuksekligi[x];
		}
		if(konum2==x){
			oyuncu2x=10+x*100;
			oyuncu2y=binayuksekligi[x];
		}
	}

	//daha önce girilen renkler ve skorlar dosyadan okunuyor.
	FILE *dosya;
	dosya=fopen("ayarlar.txt","r");
	fscanf(dosya,"%d %d %d %d %d %d",&r1,&g1,&b1,&r2,&g2,&b2);
	fclose(dosya);
	dosya=fopen("skorlar.txt","r");
	fscanf(dosya,"%d %d %d",&s1,&s2,&s3);
	fclose(dosya);
	//player 1 ekrana çizdiriliyor
	circlefill(ekran,oyuncu1x+40,oyuncu1y-15,15,makecol(r1,g1,b1));
	rectfill(ekran,oyuncu1x+20,oyuncu1y-20,oyuncu1x+60,oyuncu1y,makecol(r1,g1,b1));
	circlefill(ekran,oyuncu1x+30,oyuncu1y,5,makecol(0,0,0));
	circlefill(ekran,oyuncu1x+50,oyuncu1y,5,makecol(0,0,0));
	//player 2 ekrana çizdiriliyor
	circlefill(ekran,oyuncu2x+40,oyuncu2y-15,15,makecol(r2,g2,b2));
	rectfill(ekran,oyuncu2x+20,oyuncu2y-20,oyuncu2x+60,oyuncu2y,makecol(r2,g2,b2));
	circlefill(ekran,oyuncu2x+30,oyuncu2y,5,makecol(0,0,0));
	circlefill(ekran,oyuncu2x+50,oyuncu2y,5,makecol(0,0,0));
	
	//rüzgar yönü ve þiddeti random olarak belirleniyor.
	ruzgar=rand()%20;
	ruzgaryonu=rand()%360;
	radyanruzgaryonu=ruzgaryonu*3.14/180.0;
	rsin=sin(radyanruzgaryonu);
	rcos=cos(radyanruzgaryonu);
	//rüzgar þiddeti göstergesi ekrana çizdiriliyor.
	circlefill(ekran,500,75,40,makecol(0,0,0));
	line(ekran,500,75,500+(int)(40*cos((240-ruzgar*15)*3.14/180)),75-(int)(40*sin((240-ruzgar*15)*3.14/180)),makecol(250,250,250));
	circlefill(ekran,500,75,3,makecol(255,0,0));
	//rüzgar yönü göstergesi ekrana çizdiriliyor.
	circlefill(ekran,550,25,25,makecol(0,0,0));
	line(ekran,550,25,550+(int)(25*rcos),25-(int)(25*rsin),makecol(250,250,250));
	circlefill(ekran,550,25,3,makecol(255,0,0));
	textprintf_centre_ex(ekran,font,590,60,makecol(0,0,0),-1, "Ruzgar Yonu");

	textprintf_ex(ekran,font,10,10,makecol(0,0,0),-1, "1.oyuncu");
	textprintf_ex(ekran,font,10,20,makecol(0,0,0),-1, "Kalan Hak:");
	textprintf_ex(ekran,font,10,30,makecol(0,0,0),-1, "Skor:");
	textprintf_ex(ekran,font,700,10,makecol(0,0,0),-1, "2.oyuncu");
	textprintf_ex(ekran,font,700,20,makecol(0,0,0),-1, "Kalan Hak:");
	textprintf_ex(ekran,font,700,30,makecol(0,0,0),-1, "Skor:");
	//baþlangýç için 2 oyuncuya da 3 er hak veriliyor ve skorlar sýfýrlanýyor.
	hak1=3;
	hak2=3;
	skor1=0;
	skor2=0;
	//toplam 6 atýþ yapýlacak tek(1,3,5) atýþlarda player 1, çift(2,4,6) atýþlarda player 2'nin sýrasý.
	for(y=1;y<=6;y++){
		oyuncudegistir=false;
		if(y%2==1){
			siradakix=oyuncu1x+40;
			siradakiy=oyuncu1y-15;
			karsidakix=oyuncu2x+40;
			karsidakiy=oyuncu2y-15;
		}
		else if(y%2==0){
			siradakix=oyuncu2x+40;
			siradakiy=oyuncu2y-15;
			karsidakix=oyuncu1x+40;
			karsidakiy=oyuncu1y-15;
		}
		zaman=100; //atýþ için 10 sn süre veriliyor.
		while(oyuncudegistir!=true){
			if(mouse_x>350 && mouse_y>0 && mouse_x<450 && mouse_y<50 && mouse_b==1){ //menüye dön
				clear_to_color(screen,makecol( 64, 0, 64) );
				return false;
			}
			if(mouse_x>350 && mouse_y>50 && mouse_x<450 && mouse_y<100 && mouse_b==1){ //yenile
				return true;
			}
			if(mouse_x>350 && mouse_y>100 && mouse_x<450 && mouse_y<150 && mouse_b==1){ //çýkýþ
				exit(0);
			}
			zaman--;
			if(zaman<=0){ //10 sn lik süre içinde atýþ yapýlmazsa yapýlacaklar.
				if(siradakix==oyuncu1x+40){
					hak1--;
					oyuncudegistir=true;
				}
				if(siradakix==oyuncu2x+40){
					hak2--;
					oyuncudegistir=true;
				}
			}
			blit(ekran,screen,0,0,0,0,800,600);
			textprintf_centre_ex(screen,font,400,300,makecol(0,0,0),-1, "Zaman:%d",zaman/10);
			textprintf_ex(screen,font,90,20,makecol(0,0,0),-1, "%d",hak1);
			textprintf_ex(screen,font,780,20,makecol(0,0,0),-1, "%d",hak2);
			textprintf_ex(screen,font,50,30,makecol(0,0,0),-1, "%d",skor1);
			textprintf_ex(screen,font,740,30,makecol(0,0,0),-1, "%d",skor2);
			textprintf_centre_ex(screen,font,500,125,makecol(0,0,0),-1, "Ruzgar Hizi:%d",ruzgar);
			v0=hipotenus(siradakix,mouse_x,siradakiy,mouse_y); //mosue ile oyuncu arasýndaki uzaklýk hýz olarak belirleniyor.
			
			if(v0<=100){ //hýz 100 den küçükse atýþ yapýlabilir
				line(screen,siradakix,siradakiy,mouse_x,mouse_y,makecol(250,250,250));

				//atýþ hýzý için gösterge çiziliyor.
				circlefill(screen,300,75,40,makecol(0,0,0));
				line(screen,300,75,300+(int)(40*cos((240-v0*3)*3.14/180)),75-(int)(40*sin((240-v0*3)*3.14/180)),makecol(250,250,250));
				circlefill(screen,300,75,3,makecol(255,0,0));
				textprintf_centre_ex(screen,font,300,125,makecol(0,0,0),-1, "Atis Hizi:%d",v0);
				
				if(mouse_b==1){ //atýþ yapýlýyor.
					if(siradakix==oyuncu1x+40)
						hak1--;
					if(siradakix==oyuncu2x+40)
						hak2--;
						m_x=mouse_x;
						m_y=mouse_y;
						sina=(float)(uzaklik(m_y,siradakiy))/(float)(hipotenus(m_x,siradakix,m_y,siradakiy));
						cosa=(float)(uzaklik(m_x,siradakix))/(float)(hipotenus(m_x,siradakix,m_y,siradakiy));
						if(m_x<siradakix)
							cosa=-cosa;
						if(m_y>siradakiy)
							sina=-sina;

						t=0; //t atýþ formülündeki zamaný temsil ediyor.
					while(t<100){
						blit(ekran,screen,0,0,0,0,800,600);
						//atýþ hareketi ekranda çizdiriliyor.
						topx=m_x+(v0*t*cosa)+(int)(ruzgar*t*rcos);
						topy=m_y-(v0*t*sina-5*t*t)-(int)(ruzgar*t*rsin);
						circlefill(screen,topx,topy,10,makecol(255,0,0));

						if(topx>=karsidakix-20 && topx<=karsidakix+20 && topy>=karsidakiy-15){ //hedef vurulmuþsa yapýlacaklar.
							if(siradakix==oyuncu1x+40)
								skor1+=100+(zaman/10)*10;
							if(siradakix==oyuncu2x+40)
								skor2+=100+(zaman/10)*10;
							play_sample(bang,255,128,1000,0);
							t=100;
							//rüzgar için yeni deðerler belirleniyor
							ruzgar=rand()%20;
							ruzgaryonu=rand()%360;
							radyanruzgaryonu=ruzgaryonu*3.14/180.0;
							rsin=sin(radyanruzgaryonu);
							rcos=cos(radyanruzgaryonu);
							//rüzgar þiddeti için gösterge ekrana çizdiriliyor.
							circlefill(ekran,500,75,40,makecol(0,0,0));
							line(ekran,500,75,500+(int)(40*cos((240-ruzgar*15)*3.14/180)),75-(int)(40*sin((240-ruzgar*15)*3.14/180)),makecol(250,250,250));
							circlefill(ekran,500,75,3,makecol(255,0,0));
							//rüzgar yönü için gösterge ekrana çizdiriliyor.
							circlefill(ekran,550,25,25,makecol(0,0,0));
							line(ekran,550,25,550+(int)(25*rcos),25-(int)(25*rsin),makecol(250,250,250));
							circlefill(ekran,550,25,3,makecol(255,0,0));
						}
						else{
							for(x=0;x<=7;x++){ //kule vurulduðunda yapýlacaklar.
								if(topx>=10+x*100 && topx<=90+x*100 && topy>=binayuksekligi[x]){
									play_sample(bang,255,128,1000,0);
									circlefill(ekran,topx,topy,10,makecol(127,127,127));
									t=100;
									//rüzgar için yeni deðerler belirleniyor
									ruzgar=rand()%20;
									ruzgaryonu=rand()%360;
									radyanruzgaryonu=ruzgaryonu*3.14/180.0;
									rsin=sin(radyanruzgaryonu);
									rcos=cos(radyanruzgaryonu);
									//rüzgar þiddeti için gösterge ekrana çizdiriliyor.
									circlefill(ekran,500,75,40,makecol(0,0,0));
									line(ekran,500,75,500+(int)(40*cos((240-ruzgar*15)*3.14/180)),75-(int)(40*sin((240-ruzgar*15)*3.14/180)),makecol(250,250,250));
									circlefill(ekran,500,75,3,makecol(255,0,0));
									//rüzgar yönü için gösterge ekrana çizdiriliyor.
									circlefill(ekran,550,25,25,makecol(0,0,0));
									line(ekran,550,25,550+(int)(25*rcos),25-(int)(25*rsin),makecol(250,250,250));
									circlefill(ekran,550,25,3,makecol(255,0,0));
								}
							}
							if(topx<-10 || topx>810){ //atýþ ekrandan dýþarý çýkmýþsa yapýlacaklar
								t=100;
								//rüzgar için yeni deðerler belirleniyor
								ruzgar=rand()%20;
								ruzgaryonu=rand()%360;
								radyanruzgaryonu=ruzgaryonu*3.14/180.0;
								rsin=sin(radyanruzgaryonu);
								rcos=cos(radyanruzgaryonu);
								//rüzgar þiddeti için gösterge ekrana çizdiriliyor.
								circlefill(ekran,500,75,40,makecol(0,0,0));
								line(ekran,500,75,500+(int)(40*cos((240-ruzgar*15)*3.14/180)),75-(int)(40*sin((240-ruzgar*15)*3.14/180)),makecol(250,250,250));
								circlefill(ekran,500,75,3,makecol(255,0,0));
								//rüzgar yönü için gösterge ekrana çizdiriliyor.
								circlefill(ekran,550,25,25,makecol(0,0,0));
								line(ekran,550,25,550+(int)(25*rcos),25-(int)(25*rsin),makecol(250,250,250));
								circlefill(ekran,550,25,3,makecol(255,0,0));
							}
							
						}
						t+=0.1;
						rest(10);
					}
					oyuncudegistir=true; //sýra diðer oyuncuya geçiyor.
				}
			}
			rest(100);
		}
	}
	if(skor1>skor2){
		textprintf_centre_ex(ekran,font,400,280,makecol(250,250,250),-1,"1.Oyuncu Kazandi.");
	}
	else if(skor1<skor2){
		textprintf_centre_ex(ekran,font,400,280,makecol(250,250,250),-1,"2.Oyuncu Kazandi.");
	}
	else{
		textprintf_centre_ex(ekran,font,400,280,makecol(250,250,250),-1,"Berabere.");
	}
	if(skor1>s1){
		textprintf_centre_ex(ekran,font,400,300,makecol(250,250,250),-1,"1.Oyuncu Yuksek Skor Yapti.");
		s3=s2;
		s2=s1;
		s1=skor1;
	}
	else if(skor1>s2){
		textprintf_centre_ex(ekran,font,400,300,makecol(250,250,250),-1,"1.Oyuncu Yuksek Skor Yapti.");
		s3=s2;
		s2=skor1;
	}
	else if(skor1>s3){
		textprintf_centre_ex(ekran,font,400,300,makecol(250,250,250),-1,"1.Oyuncu Yuksek Skor Yapti.");
		s3=skor1;
	}
	if(skor2>s1){
		textprintf_centre_ex(ekran,font,400,320,makecol(250,250,250),-1,"2.Oyuncu Yuksek Skor Yapti.");
		s3=s2;
		s2=s1;
		s1=skor2;
	}
	else if(skor2>s2){
		textprintf_centre_ex(ekran,font,400,320,makecol(250,250,250),-1,"2.Oyuncu Yuksek Skor Yapti.");
		s3=s2;
		s2=skor2;
	}
	else if(skor2>s3){
		textprintf_centre_ex(ekran,font,400,320,makecol(250,250,250),-1,"2.Oyuncu Yuksek Skor Yapti.");
		s3=skor2;
	}
	//skorlar dosyaya yazýlýyor.
	dosya=fopen("skorlar.txt","w");
	fprintf(dosya,"%d\n%d\n%d",s1,s2,s3);
	fclose(dosya);
	
	textprintf_ex(ekran,font,90,20,makecol(0,0,0),-1, "%d",hak1);
	textprintf_ex(ekran,font,780,20,makecol(0,0,0),-1, "%d",hak2);
	textprintf_ex(ekran,font,50,30,makecol(0,0,0),-1, "%d",skor1);
	textprintf_ex(ekran,font,740,30,makecol(0,0,0),-1, "%d",skor2);

	while(mouse_b!=1){ //ekrana týklanana kadar bekle
		blit(ekran,screen,0,0,0,0,800,600);
		rest(100);
	}

	clear_to_color(screen,makecol( 64, 0, 64) ); //ana menüye dön
	return false;
}

void ayarlar(void){
	
	BITMAP *ekran;
	int r1,g1,b1,r2,g2,b2;

	ekran=create_bitmap(800,600);
	clear_to_color(ekran,makecol( 64, 0, 64) );
	//eski renk deðerleri okunuyor.
	FILE *dosya;
	dosya=fopen("ayarlar.txt","r");
	fscanf(dosya,"%d %d %d %d %d %d",&r1,&g1,&b1,&r2,&g2,&b2);
	fclose(dosya);

	textprintf_centre_ex(ekran,font,740,50,makecol(255, 255, 255),-1, "MENU");
	rect(ekran,710,30,770,70,makecol(255,255,255));
	textprintf_centre_ex(ekran,font,740,100,makecol(255, 255, 255),-1, "KAYDET");
	rect(ekran,710,80,770,120,makecol(255,255,255));
	textprintf_centre_ex(ekran,font,150,100,makecol(255, 255, 255),-1, "1.Oyuncu");
	rect(ekran,100,80,200,120,makecol(255,255,255));
	textprintf_centre_ex(ekran,font,125,160,makecol(255, 255, 255),-1, "Red");
	rect(ekran,100,150,150,170,makecol(255,255,255));
	textprintf_centre_ex(ekran,font,125,200,makecol(255, 255, 255),-1, "Green");
	rect(ekran,100,190,150,210,makecol(255,255,255));
	textprintf_centre_ex(ekran,font,125,240,makecol(255, 255, 255),-1, "Blue");
	rect(ekran,100,230,150,250,makecol(255,255,255));
	textprintf_centre_ex(ekran,font,150,300,makecol(255, 255, 255),-1, "Renk");
	textprintf_centre_ex(ekran,font,450,100,makecol(255, 255, 255),-1, "2.Oyuncu");
	rect(ekran,400,80,500,120,makecol(255,255,255));
	textprintf_centre_ex(ekran,font,425,160,makecol(255, 255, 255),-1, "Red");
	rect(ekran,400,150,450,170,makecol(255,255,255));
	textprintf_centre_ex(ekran,font,425,200,makecol(255, 255, 255),-1, "Green");
	rect(ekran,400,190,450,210,makecol(255,255,255));
	textprintf_centre_ex(ekran,font,425,240,makecol(255, 255, 255),-1, "Blue");
	rect(ekran,400,230,450,250,makecol(255,255,255));
	textprintf_centre_ex(ekran,font,450,300,makecol(255, 255, 255),-1, "Renk");

	while(1){
		rectfill(ekran,180,150,200,170,makecol(r1,0,0));
		rectfill(ekran,180,190,200,210,makecol(0,g1,0));
		rectfill(ekran,180,230,200,250,makecol(0,0,b1));
		rectfill(ekran,480,150,500,170,makecol(r2,0,0));
		rectfill(ekran,480,190,500,210,makecol(0,g2,0));
		rectfill(ekran,480,230,500,250,makecol(0,0,b2));
		rectfill(ekran,100,310,200,380,makecol(r1,g1,b1));
		rectfill(ekran,400,310,500,380,makecol(r2,g2,b2));
		blit(ekran,screen,0,0,0,0,800,600);
		if(mouse_x>710 && mouse_y>30 && mouse_x<770 && mouse_y<70 && mouse_b==1){
			clear_to_color(screen,makecol( 64, 0, 64) ); //menüye dön
			break;
		}
		if(mouse_x>100 && mouse_y>150 && mouse_x<150 && mouse_y<170 && mouse_b==1){
			if(r1<255) r1++;
			else r1=0;
		}
		if(mouse_x>100 && mouse_y>190 && mouse_x<150 && mouse_y<210 && mouse_b==1){
			if(g1<255) g1++;
			else g1=0;
		}
		if(mouse_x>100 && mouse_y>230 && mouse_x<150 && mouse_y<250 && mouse_b==1){
			if(b1<255) b1++;
			else b1=0;
		}
		if(mouse_x>400 && mouse_y>150 && mouse_x<450 && mouse_y<170 && mouse_b==1){
			if(r2<255) r2++;
			else r2=0;
		}
		if(mouse_x>400 && mouse_y>190 && mouse_x<450 && mouse_y<210 && mouse_b==1){
			if(g2<255) g2++;
			else g2=0;
		}
		if(mouse_x>400 && mouse_y>230 && mouse_x<450 && mouse_y<250 && mouse_b==1){
			if(b2<255) b2++;
			else b2=0;
		}
		if(mouse_x>710 && mouse_y>80 && mouse_x<770 && mouse_y<120 && mouse_b==1){
			dosya=fopen("ayarlar.txt","w");
			fprintf(dosya,"%d\n%d\n%d\n%d\n%d\n%d",r1,g1,b1,r2,g2,b2);
			fclose(dosya);
			textprintf_centre_ex(screen,font,400,500,makecol(255, 255, 255),-1, "Ayarlar Kaydedildi.");
		}
		rest(10);
	}
}

void yuksekskor(void){

	BITMAP *ekran;
	int x,skor;
	FILE *dosya;
	dosya=fopen("skorlar.txt","r");

	ekran=create_bitmap(800,600);
	clear_to_color(ekran,makecol( 64, 0, 64) );
	textprintf_centre_ex(ekran,font,740,50,makecol(255, 255, 255),-1, "MENU");
	rect(ekran,710,30,770,70,makecol(255,255,255));
	
	//skorlar ekrana yazdýrýlýyor.
	for(x=0;x<3;x++){
		
		fscanf(dosya,"%d",&skor);

		textprintf_centre_ex(ekran,font,400,200+x*100,makecol(255, 255, 255),-1, "%d",skor);
		rect(ekran,300,160+x*100,500,240+x*100,makecol(255,255,255));
	}

	fclose(dosya);

	while(1){
		blit(ekran,screen,0,0,0,0,800,600);
		if(mouse_x>710 && mouse_y>30 && mouse_x<770 && mouse_y<70 && mouse_b==1){ //ana menüye dön
		clear_to_color(screen,makecol( 64, 0, 64) );
		break;
		}
		rest(100);
	}
}
