#include "../interface/GenRunNtuplizer.h"

//===================================================================================================================
GenRunNtuplizer::GenRunNtuplizer( std::vector< edm::EDGetTokenT< GenRunInfoProduct > > tokens, NtupleBranches* nBranches )
   : CandidateNtuplizer( nBranches )
   , genrunToken_( tokens[0] )
{

}

//===================================================================================================================
GenRunNtuplizer::~GenRunNtuplizer( void )
{

}

//===================================================================================================================
void GenRunNtuplizer::fillBranches( edm::Run const & run, const edm::EventSetup& iSetup ){

 
   // edm::EDGetTokenT< GenRunInfoProduct > genrunToken_;
  // edm::Handle< GenRunInfoProduct >  genrunInfo_; 

//  run.getByToken(genrunToken_, genrunInfo_);

// edm::LogError("") << "Xsection: " << genrunInfo_->crossSection(); 
 
//  nBranches_->genWeight=geneventInfo_->weight();
}
