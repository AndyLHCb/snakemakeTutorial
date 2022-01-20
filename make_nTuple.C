void make_nTuple()
{
	UInt_t nameNo = 0;
	TFile* out = nullptr;

	do
	{
		const TString nTupleName = Form("nTuple%uDT.root",nameNo);
		out = TFile::Open(nTupleName,"CREATE");
		nameNo++;
		if(nameNo > 4){return;}
	}while(not out);
	nameNo--;
	std::cout << "File number:" << nameNo << std::endl;

	const ULong64_t nEntries = 10'000;

	//Make a tree with 2 branchs in the nTuple
	TTree*   tree   = new TTree("Tree","nTuple for Snakemake tutorial");
	Double_t dtMeas{0}, dtTrue{0};
	TBranch* branchMeas = tree->Branch("B_DT_Measured",&dtMeas,"B_DT_Measured/D");
	TBranch* branchTrue = tree->Branch("B_DT_True",    &dtTrue,"B_DT_True/D");

	//Fill the branch up
	for(ULong64_t iEvt = 0; iEvt < nEntries; ++iEvt)
	{
		if(iEvt % 1'000 == 0){std::cout << "Writing event " << iEvt << "/" << nEntries << std::endl;}
		dtTrue =          gRandom->Exp(1.519); //DT in ps
		dtMeas = dtTrue + gRandom->Gaus(0., 0.04); //distorted by DTR

		tree->Fill();
	}
	tree->Write();
	out ->Write();
	out ->Close();
	return;
}
