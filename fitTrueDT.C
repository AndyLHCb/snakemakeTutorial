void fitTrueDT(TString inputFileName)
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

	TCanvas* c1 = new TCanvas();
	TH1D* DT_hist = new TH1D("DT_hist","Decay Time (ps)",60,0.,10.);

	inputTree->Draw("B_DT_True >> DT_hist");

	DT_hist->Fit("expo");

	c1->SaveAs("TrueDTfit.pdf");

	inputFile->Close();

	return;
}
