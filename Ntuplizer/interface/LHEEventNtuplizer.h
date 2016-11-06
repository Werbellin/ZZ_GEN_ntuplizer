#ifndef LHEEventNtuplizer_H
#define LHEEventNtuplizer_H

#include "../interface/CandidateNtuplizer.h"

class LHEEventNtuplizer : public CandidateNtuplizer {

public:
  LHEEventNtuplizer( std::vector< edm::EDGetTokenT< LHEEventProduct > > tokens, NtupleBranches* nBranches );
  ~LHEEventNtuplizer( void );
  
  void fillBranches( edm::Event const & event, const edm::EventSetup& iSetup );
  
private:
   edm::EDGetTokenT< LHEEventProduct > LHEEventToken_; 
     
   edm::Handle< LHEEventProduct >  LHEEventProduct_;
      
};

#endif // LHEEventNtuplizer_H
