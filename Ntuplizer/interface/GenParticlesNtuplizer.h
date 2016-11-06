#ifndef GenParticlesNtuplizer_H
#define GenParticlesNtuplizer_H

#include "../interface/CandidateNtuplizer.h"



class GenParticlesNtuplizer : public CandidateNtuplizer {

public:
  GenParticlesNtuplizer( std::vector<edm::EDGetTokenT<reco::GenParticleCollection>> tokens, edm::EDGetTokenT<reco::CandidateView>, NtupleBranches* nBranches );
  ~GenParticlesNtuplizer( void ); 

  void fillBranches( edm::Event const & event, const edm::EventSetup& iSetup );

private:
   edm::EDGetTokenT<reco::GenParticleCollection> genParticlesToken_;
   edm::Handle< reco::GenParticleCollection >  genParticles_;
 
   edm::EDGetTokenT<reco::CandidateView> genParticlesTokenC_;
   edm::Handle< reco::CandidateView >  genParticlesC_;
      
};

#endif // GenParticlesNtuplizer_H
