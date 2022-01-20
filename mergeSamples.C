void mergeSamples(TString iFile1, TString iFile2)
{
	std::vector<TString> filenames = {iFile1, iFile2};
	TString oFileName{iFile1};
	oFileName.ReplaceAll( oFileName("[0-9]+"), "Combined" );
	TFile* oFile = TFile::Open(oFileName,"CREATE");
	if(not oFile)
	{
		std::cerr << "ERROR: output file already exists! aborting..." << std::endl;
		gSystem->Exit(EXIT_FAILURE);
	}

	TTree* oTree = nullptr;
	
	for(TString& iFileName : filenames)
	{
		std::cout << "Processing file: " << iFileName << std::endl;
		//Get input Tree
		TFile* iFile = TFile::Open(iFileName);
		if(not iFile)
		{
			std::cerr << "ERROR: input file " << iFileName << " not found! aborting..." << std::endl;
			gSystem->Exit(EXIT_FAILURE);
		}
		TTree* iTree = dynamic_cast<TTree*>(iFile->Get("Tree"));
		if(not iTree)
		{
			std::cerr << "ERROR: Tree not found in file " << iFileName << "! aborting..." << std::endl;
			gSystem->Exit(EXIT_FAILURE);
		}
		
		//Set up output tree and branches
		oFile->cd();
		Int_t fileNo = ((TString)iFileName("[0-9]+")).Atoi();
		if(iFileName == filenames[0])
		{
			oTree = iTree -> CloneTree(0);
			oTree -> Branch("fileNo",&fileNo,"fileNo/I");
		}
		else
		{
			oTree -> CopyAddresses(iTree);
			oTree -> SetBranchAddress("fileNo", &fileNo);
		}

		//Event loop
		const ULong64_t nEntries = iTree->GetEntries();
		for(ULong64_t iEvt = 0; iEvt < nEntries; ++iEvt)
		{
			if(iEvt % 1000 == 0){std::cout << "Processing event " << iEvt << "/" << nEntries << std::endl;}

			iTree -> GetEntry(iEvt);
			oTree -> Fill();
		}
		if(iFileName == filenames[0])
		{
			iTree->GetListOfClones()->Remove(oTree);
		}
		iTree->ResetBranchAddresses();
		oTree->ResetBranchAddresses();

		oTree->Write();
		oFile->cd();

		iFile->Close();
	}
	oFile->Write();
	oFile->Close();

	return;
}
