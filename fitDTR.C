void fitDTR(TString inputFileName)
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
	TH1D* DTR_hist = new TH1D("DTR_hist","Decay Time Resolution (ps)",60,-0.1,0.1);

	inputTree->Draw("DTR >> DTR_hist");

	DTR_hist->Fit("gaus");

	c1->SaveAs("DTRfit.pdf");

	inputFile->Close();

	return;
}
