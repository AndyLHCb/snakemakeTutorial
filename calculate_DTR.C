void calculate_DTR(const TString inputFileName)
{
	//Input setup
	TFile* inputFile = TFile::Open(inputFileName);
	if(not inputFile)
	{
		std::cerr << "ERROR: input file not found! Aborting..." << std::endl;
		gSystem->Exit(EXIT_FAILURE);
	}
	TTree* inputTree = dynamic_cast<TTree*>(inputFile->Get("Tree"));
	if(not inputTree)
	{
		std::cerr << "ERROR: input tree not found! Aborting..." << std::endl;
		gSystem->Exit(EXIT_FAILURE);
	}
	Double_t DT_True{0.}, DT_Meas{0.};
	inputTree->SetBranchAddress("B_DT_True",    &DT_True);
	inputTree->SetBranchAddress("B_DT_Measured",&DT_Meas);

	//Output setup
	TString outputFileName{inputFileName};
	outputFileName.ReplaceAll(".root","-withDTR.root");

	TFile* outputFile = TFile::Open(outputFileName,"CREATE");
	if(not outputFile)
	{
		std::cerr<< "ERROR: output file name already exists! Aborting..." << std::endl;
		gSystem->Exit(EXIT_FAILURE);
	}
	TTree* outputTree = inputTree->CloneTree(0);
	Double_t DTR{-999.};
	outputTree->Branch("DTR",&DTR,"DTR/D");

	//Event loop
	const ULong64_t nEntries = inputTree->GetEntries();
	for(ULong64_t iEvt{0}; iEvt < nEntries; ++iEvt)
	{
		if(iEvt % 1'000 == 0){std::cout << "Processed event: " << iEvt << "/" << nEntries << std::endl;}
		inputTree->GetEntry(iEvt);
		DTR = DT_True - DT_Meas;

		outputTree->Fill();
	}
	outputTree->Write();
	outputFile->Write();

	outputFile->Close();
	inputFile ->Close();
	
	return;
}
