void ReadMacro(TString macroName, Int_t *parI, Double_t *parD, TString *parS, Int_t MaxN=10){
  
  const int MN = MaxN;
  const Int_t Max = 1e9;
  ifstream macro;
  macro.open(macroName.Data());
  TString key,value;
  Int_t N = 0;
  Int_t M = 0;
  char line[256];
  
  parI[2] = 0;
  parI[3] = 1;
  
  for(int i=1; i<MN; i++) parD[2*MN+i] = 1.;
  
  if (macro.is_open() && !macro.eof()){
    do{
      macro.ignore(Max,'\n');
      macro >> key;
      if (key.BeginsWith("#")) {
	macro.ignore(Max,'\n');
      }
      else{

	macro >> value;
	
	if (key=="RUN_ID")     {parI[0] = value.Atoi();}
	if (key=="RUN_TIME")   {parD[0] = value.Atof();}
	if (key=="DARK_NOISE") {parD[1] = value.Atof();}	
	if (key=="OUTPUT_FILE"){parS[0] = value;}	
        if (key=="OUTPUT_TIME"){parS[3] = value;}
        if (key=="OUTPUT_PLOT"){parS[4] = value;}
	if (key=="INPUT_LOCATION") {parS[1] = value;}	
	if (key=="OUTPUT_LOCATION"){parS[2] = value;}	
	if (key=="SRC")      {N++;}
	if (key=="SRC_NAME") {parS[1*MN+N] = value;}
	if (key=="SRC_FREQ") {parD[1*MN+N] = value.Atof();}
	if (key=="SRC_RATIO"){parD[2*MN+N] = value.Atof();}
	if (key=="SRC_FILE") {parS[2*MN+N] = value;}
	
	if (key=="FILTER_NOHIT" && value.Atoi()==1){parI[2] = 1;}
	if (key=="AVERAGE_JUMP") {parI[3] = value.Atoi();}	
      }
    }while(!macro.eof());
    
    parI[1] = N;
    if (parI[3]<1) parI[3]=1;    
    parD[2] = 1./((double) parI[3]);
    parI[3] = parI[3]%2;
    
  } else {cout << "Error :: File " << macroName << " broken or not found." << endl;}
  macro.close();  
}
