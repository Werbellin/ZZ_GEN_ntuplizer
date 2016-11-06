#ifndef GenRunNtuplizer_H
#define GenRunNtuplizer_H

#include "../interface/CandidateNtuplizer.h"

class GenRunNtuplizer : public CandidateNtuplizer {

public:
  GenRunNtuplizer( std::vector< edm::EDGetTokenT< GenRunInfoProduct > > tokens, NtupleBranches* nBranches );
  ~GenRunNtuplizer( void );
  
  void fillBranches( edm::Run const & run, const edm::EventSetup& iSetup );
  
private:
   edm::EDGetTokenT< GenRunInfoProduct > genrunToken_; 
     
   edm::Handle< GenRunInfoProduct >  genrunInfo_;
      
};

#endif // GenRunNtuplizer_H
