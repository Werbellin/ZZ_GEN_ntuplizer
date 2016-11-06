#include "../interface/LHEEventNtuplizer.h"

//===================================================================================================================
LHEEventNtuplizer::LHEEventNtuplizer( std::vector< edm::EDGetTokenT< LHEEventProduct > > tokens, NtupleBranches* nBranches )
   : CandidateNtuplizer( nBranches )
   , LHEEventToken_( tokens[0] )
{

}

//===================================================================================================================
LHEEventNtuplizer::~LHEEventNtuplizer( void )
{

}

//===================================================================================================================
void LHEEventNtuplizer::fillBranches( edm::Event const & event, const edm::EventSetup& iSetup ){

  event.getByToken(LHEEventToken_, LHEEventProduct_);  


  const LHEEventProduct* product = LHEEventProduct_.product();
//  edm::LogWarning("blubb") << "weights" << product->weights().size();
  //std::vector<std::pair<int, float> > weights;



   
  std::vector<float > weights;
  for(size_t i = 0; i < 11 ; ++i) {
//    weights.push_back(make_pair(stoi(product->weights().at(i).id), product->weights().at(i).wgt));
    weights.push_back(product->weights().at(i).wgt);
  }
    nBranches_->originalXWGTUP = product->originalXWGTUP();
    nBranches_->weights = weights;  
  //nBranches_->genWeight=geneventInfo_->weight();
  //nBranches_->qScale=geneventInfo_->qScale();
  //nBranches_->PDF_x.push_back((geneventInfo_->pdf()->x).first);
  //nBranches_->PDF_x.push_back((geneventInfo_->pdf()->x).second);
  //nBranches_->PDF_xPDF.push_back((geneventInfo_->pdf()->xPDF).first);
  //nBranches_->PDF_xPDF.push_back((geneventInfo_->pdf()->xPDF).second);
  //nBranches_->PDF_id.push_back((geneventInfo_->pdf()->id).first);
  //nBranches_->PDF_id.push_back((geneventInfo_->pdf()->id).second);

}
