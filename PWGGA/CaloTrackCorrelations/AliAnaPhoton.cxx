/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes hereby granted      *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// --- ROOT system ---
#include <TH3F.h>
#include <TH2F.h>
#include <TClonesArray.h>
#include <TObjString.h>
#include "TDatabasePDG.h"
#include <TCustomBinning.h>

// --- AliRoot/Analysis system ---
#include "AliVParticle.h"
#include "AliVCluster.h"
#include "AliMixedEvent.h"
#include "AliAODEvent.h"
#include "AliESDEvent.h"
#include "AliMCEvent.h"
#include "AliGenPythiaEventHeader.h"

// --- CaloTrackCorr system ---
#include "AliAnaPhoton.h"
#include "AliCaloTrackReader.h"
#include "AliCaloPID.h"
#include "AliMCAnalysisUtils.h"
#include "AliFiducialCut.h"

// --- Detectors ---
#include "AliPHOSGeoUtils.h"
#include "AliEMCALGeometry.h"

/// \cond CLASSIMP
ClassImp(AliAnaPhoton) ;
/// \endcond

//____________________________
/// Default constructor.
/// Initialize parameters with default values.
//____________________________
AliAnaPhoton::AliAnaPhoton() :
AliAnaCaloTrackCorrBaseClass(),
fMinDist(0.),                 fMinDist2(0.),                fMinDist3(0.),
fRejectTrackMatch(0),         fFillTMHisto(kFALSE),         
fFillTMHistoAfterCut(0),      fFillTMHistoTrackPt(0),
fTimeCutMin(-10000),          fTimeCutMax(10000),
fNCellsCut(0),
fNLMCutMin(-1),               fNLMCutMax(10),
fFillSSHistograms(0),         fFillSSPerSMHistograms(0),    fFillEMCALRegionSSHistograms(0), 
fFillConversionVertexHisto(0),fFillOnlySimpleSSHisto(1),
fFillSSNLocMaxHisto(0),
fFillTrackMultHistograms(0),  fFillControlClusterContentHisto(0),
fSeparateConvertedDistributions(0),
fNOriginHistograms(9),        fNPrimaryHistograms(5),
fMomentum(),                  fMomentum2(),
fPrimaryMom(),                fPrimaryMom2(),              fProdVertex(),
fConstantTimeShift(0),        fFillEBinAcceptanceHisto(0),
fStudyActivityNearCluster(0), fCheckOverlaps(0),           fFillOnlyPtHisto(0),
// Histograms

// Control histograms
fhNCellsE(0),                 fhNLocMaxE(0),
fhNCellsECluster(0),          fhNLocMaxECluster(0),
fhNCellsEClusterNeutral(0),   fhNLocMaxEClusterNeutral(0),
fhNCellsCentralityE(0),                 fhNLocMaxCentralityE(0),
fhNCellsCentralityECluster(0),          fhNLocMaxCentralityECluster(0),
fhNCellsCentralityEClusterNeutral(0),   fhNLocMaxCentralityEClusterNeutral(0),
fhCellsE(0),
fhMaxCellDiffClusterE(0),     fhTimePt(0),                  fhEtaPhi(0),

fhEPhoton(0),                 fhPtPhoton(0),
fhPhiPhoton(0),               fhEtaPhoton(0),
fhEtaPhiPhoton(0),            fhEtaPhi05Photon(0),
fhEnergyEtaPhi(0),            fhEnergyColRow(0), 
fhEnergyEtaPhiPID(0),         fhEnergyColRowPID(0),
fhPtCentralityPhoton(0),      fhPtEventPlanePhoton(0),

// Shower shape histograms
fhDispE(0),                   fhDispPt(0),                  
fhLam0E(0),                   fhLam0ECluster(0),           
fhLam0EClusterNeutral(0),     fhLam0Pt(0),        
fhLam1E(0),                   fhLam1Pt(0),
fhLam0NLocMaxECluster(0),     fhLam0NLocMaxEClusterNeutral(0),    
  
fhLam0CentralityE(0),         fhLam0CentralityECluster(0),           
fhLam0CentralityEClusterNeutral(0),       
fhLam0NLocMaxEClusterPerCen(0),     
fhLam0NLocMaxEClusterNeutralPerCen(0),   

fhDispETRD(0),                fhLam0ETRD(0),                fhLam0PtTRD(0),     fhLam1ETRD(0),
fhDispETM(0),                 fhLam0ETM(0),                 fhLam0PtTM(0),      fhLam1ETM(0),
fhDispETMTRD(0),              fhLam0ETMTRD(0),              fhLam0PtTMTRD(0),   fhLam1ETMTRD(0),

fhNCellsLam0LowE(0),          fhNCellsLam1LowE(0),          fhNCellsDispLowE(0),
fhNCellsLam0HighE(0),         fhNCellsLam1HighE(0),         fhNCellsDispHighE(0),

fhEtaLam0LowE(0),             fhPhiLam0LowE(0),
fhEtaLam0HighE(0),            fhPhiLam0HighE(0),
fhLam0DispLowE(0),            fhLam0DispHighE(0),
fhLam1Lam0LowE(0),            fhLam1Lam0HighE(0),
fhDispLam1LowE(0),            fhDispLam1HighE(0),
fhDispEtaE(0),                fhDispPhiE(0),
fhSumEtaE(0),                 fhSumPhiE(0),                 fhSumEtaPhiE(0),
fhDispEtaPhiDiffE(0),         fhSphericityE(0),
fhDispSumEtaDiffE(0),         fhDispSumPhiDiffE(0),

// MC histograms
//fhMCPhotonELambda0NoOverlap(0),       fhMCPhotonELambda0TwoOverlap(0),      fhMCPhotonELambda0NOverlap(0),
// Embedding
fhEmbeddedSignalFractionEnergy(0),    fhEmbeddedSignalFractionEnergyPerCentrality(0),
fhEmbeddedPhotonFractionEnergy(0),    fhEmbeddedPhotonFractionEnergyM02(0),
fhEmbeddedPi0FractionEnergy(0),       fhEmbeddedPi0FractionEnergyM02(0),

fhEOverPAfterResidualCut(0),                  fhEOverPTrackPtAfterResidualCut(0),
fhTrackMatchedDEtaDPhiPosAfterEOverPCut(0),   
fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCut(0),

fhTrackMatchedDEtaDPhiPosPerCen(0), 
fhTrackMatchedDEtaDPhiPosTrackPtPerCen(0),   
fhTrackMatchedDEtaDPhiPosAfterEOverPCutPerCen(0), 
fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCutPerCen(0),
fhEOverPCentrality(0), 
fhEOverPCentralityTrackPt(0), 
fhEOverPCentralityAfterResidualCut(0), 
fhEOverPCentralityTrackPtAfterResidualCut(0),      

fhTimePtPhotonNoCut(0),               fhTimePtPhotonSPD(0),
fhTimeNPileUpVertSPD(0),              fhTimeNPileUpVertTrack(0),
fhPtPhotonNPileUpSPDVtx(0),           fhPtPhotonNPileUpTrkVtx(0),
fhPtPhotonNPileUpSPDVtxTimeCut(0),    fhPtPhotonNPileUpTrkVtxTimeCut(0),
fhPtPhotonNPileUpSPDVtxTimeCut2(0),   fhPtPhotonNPileUpTrkVtxTimeCut2(0),

fhEClusterSM(0),                      fhEPhotonSM(0),
fhPtClusterSM(0),                     fhPtPhotonSM(0),
fhPtPhotonCentralitySM(0),

fhMCConversionVertex(0),              fhMCConversionVertexTRD(0),
//fhDistanceAddedPhotonAddedPrimarySignal  (0), fhDistanceHijingPhotonAddedPrimarySignal  (0),
//fhDistanceAddedPhotonAddedSecondarySignal(0), fhDistanceHijingPhotonAddedSecondarySignal(0),
//fhDistanceAddedPhotonHijingSecondary(0)
fhLocalRegionClusterEnergySumHijing2(0), 
fhLocalRegionClusterMultiplicityHijing2(0),
fhLocalRegionClusterEnergySumPerCentralityHijing2(0),
fhLocalRegionClusterMultiplicityPerCentralityHijing2(0), 
fhDistance2AddedSignals(0),           fhDistanceAddedSignalsHijing(0),
fhDistance2Hijing(0)
{
  for(Int_t i = 0; i < fgkNmcTypes; i++)
  {
    fhMCPt     [i] = 0;
    fhMCE      [i] = 0;
    fhMCPhi    [i] = 0;
    fhMCEta    [i] = 0;
    fhMCDeltaE [i] = 0;
    fhMCDeltaPt[i] = 0;
    fhMC2E     [i] = 0;
    fhMC2Pt    [i] = 0;
  }
  
  for(Int_t i = 0; i < fgkNmcPrimTypes; i++)
  {
    fhPtPrimMC [i] = 0;
    fhEPrimMC  [i] = 0;
    fhPhiPrimMC[i] = 0;
    fhEtaPrimMC[i] = 0;
    fhYPrimMC  [i] = 0;
    
    fhPtPrimMCAcc [i] = 0;
    fhEPrimMCAcc  [i] = 0;
    fhPhiPrimMCAcc[i] = 0;
    fhEtaPrimMCAcc[i] = 0;
    fhYPrimMCAcc  [i] = 0;
  }
  
  for(Int_t i = 0; i < 7; i++)
  {
    fhDispEtaDispPhi[i] = 0;
    fhLambda0DispPhi[i] = 0;
    fhLambda0DispEta[i] = 0;
    
    fhPtPhotonPileUp[i] = 0;
    fhClusterTimeDiffPhotonPileUp [i] = 0;
    
    for(Int_t j = 0; j < fgkNssTypes; j++)
    {
      fhMCDispEtaDispPhi[i][j] = 0;
      fhMCLambda0DispEta[i][j] = 0;
      fhMCLambda0DispPhi[i][j] = 0;
    }
  }
  
  for(Int_t i = 0; i < fgkNssTypes; i++)
  {
    fhMCELambda0    [i]                  = 0;
    fhMCPtLambda0   [i]                  = 0;
    fhMCELambda1    [i]                  = 0;
    fhMCEDispersion [i]                  = 0;
    fhMCNCellsE     [i]                  = 0;
    fhMCMaxCellDiffClusterE[i]           = 0;
    
    for(Int_t iover = 0 ; iover < 3; iover++)
      fhMCPtLambda0Overlaps[i][iover] = 0;
    
    fhMCLambda0vsClusterMaxCellDiffE0[i] = 0;
    fhMCLambda0vsClusterMaxCellDiffE2[i] = 0;
    fhMCLambda0vsClusterMaxCellDiffE6[i] = 0;
    fhMCNCellsvsClusterMaxCellDiffE0 [i] = 0;
    fhMCNCellsvsClusterMaxCellDiffE2 [i] = 0;
    fhMCNCellsvsClusterMaxCellDiffE6 [i] = 0;
    
    fhMCEDispEta       [i]               = 0;
    fhMCEDispPhi       [i]               = 0;
    fhMCESumEtaPhi     [i]               = 0;
    fhMCEDispEtaPhiDiff[i]               = 0;
    fhMCESphericity    [i]               = 0;
  }
  
  for(Int_t i = 0; i < fgkNClusterCuts; i++)
  {
    fhClusterCutsE [i] = 0;
    fhClusterCutsPt[i] = 0;
  }
  
  for(Int_t icut = 0; icut < 10; icut++)
  {
    fhPtPhotonNTracks    [icut] = 0;  
    fhPtPhotonSumPtTracks[icut] = 0;     
  }
  
  // Track matching residuals
  for(Int_t i = 0; i < 2; i++)
  {
    fhTrackMatchedDEtaDPhiNeg[i] = 0;
    fhTrackMatchedDEtaDPhiPos[i] = 0;

    fhTrackMatchedDEtaDPhiNegTrackPt[i] = 0;
    fhTrackMatchedDEtaDPhiPosTrackPt[i] = 0;
    
    fhTrackMatchedDEtaTRD[i] = 0;             fhTrackMatchedDPhiTRD[i] = 0;
    fhTrackMatchedDEtaMCOverlap[i] = 0;       fhTrackMatchedDPhiMCOverlap[i] = 0;
    fhTrackMatchedDEtaMCNoOverlap[i] = 0;     fhTrackMatchedDPhiMCNoOverlap[i] = 0;
    fhTrackMatchedDEtaMCConversion[i] = 0;    fhTrackMatchedDPhiMCConversion[i] = 0;
    fhTrackMatchedMCParticleBeforeTM[i] = 0;  fhTrackMatchedMCParticleTrackPtBeforeTM[i] = 0;          
    fhTrackMatchedMCParticle[i] = 0;          fhTrackMatchedMCParticleTrackPt[i] = 0;          
    fhTrackMatchedMCParticleConverted[i] = 0;
    fhTrackMatchedMCParticleVsEOverP[i] = 0;  fhTrackMatchedMCParticleVsErecEgen[i] = 0;
    fhTrackMatchedMCParticleVsNOverlaps[i] = 0;          
    fhdEdx[i] = 0;                            fhEOverP[i] = 0;
    fhdEdxTrackPt[i] = 0;                     fhEOverPTrackPt[i] = 0;
    fhEOverPTRD[i] = 0;
  }
  
  for(Int_t i = 0; i < 4; i++)
  {
    fhMCParticle           [i] = 0;          
    fhMCParticleConverted  [i] = 0;
    fhMCParticleVsErecEgen [i] = 0;
    fhMCParticleVsNOverlaps[i] = 0;          
  }
  
  for(Int_t i = 0; i < 6; i++) 
  {
    fhMCConversionLambda0Rcut   [i] = 0;            
    fhMCConversionLambda0RcutTRD[i] = 0;          
    fhMCConversionLambda1Rcut   [i] = 0;            
    fhMCConversionLambda1RcutTRD[i] = 0;            
  }
  
  for(Int_t ieta = 0; ieta < 4; ieta++) 
  {  
    for(Int_t iphi = 0; iphi < 3; iphi++) 
    {
//      fhLam0EMCALRegion   [ieta][iphi] = 0;
//      fhLam0EMCALRegionTRD[ieta][iphi] = 0;
//      
//      for(Int_t i = 0; i < 6; i++) 
//      {
//        fhLam0EMCALRegionMCConvRcut   [ieta][iphi][i] = 0;
//        fhLam0EMCALRegionTRDMCConvRcut[ieta][iphi][i] = 0;
//      }
      for(Int_t ism =0; ism < 20; ism++)
      {
        fhLam0EMCALRegionPerSM[ieta][iphi][ism] = 0; 
        fhLam1EMCALRegionPerSM[ieta][iphi][ism] = 0; 
      }
    }
  }

  for(Int_t il0 = 0; il0 < 2; il0++)
  {
    for(Int_t i = 0; i < 7; i++) 
    {
      fhEtaPhiLam0BinPtBin          [il0][i] = 0 ;
      fhEtaPhiLargeTimeInClusterCell[il0][i] = 0 ;    

      fhColRowLam0BinPtBin          [il0][i] = 0 ;              
      fhColRowLam0BinPtBinWeighted  [il0][i] = 0 ;  
      fhColRowLam0BinPtBinLargeTime [il0][i] = 0 ;  
      fhCellClusterIndexEAndTime    [il0][i] = 0 ; 
      fhCellClusterEAndTime         [il0][i] = 0 ;
      fhCellClusterEFracAndTime     [il0][i] = 0 ;
    }
    
    for(Int_t ism =0; ism < 20; ism++)
    {
      fhLam1Lam0BinPerSM                [il0][ism] = 0; 
      fhTimeLam0BinPerSM                [il0][ism] = 0;   
      fhTimeLam0BinPerSMWeighted        [il0][ism] = 0;   
      fhDTimeLam0BinPerSM               [il0][ism] = 0;   
      fhDTimeLam0BinPerSMWeighted       [il0][ism] = 0;   
      fhCellClusterEFracLam0BinPerSM    [il0][ism] = 0;
//      fhCellClusterEFracLam0BinPerSMWeighted[il0][ism] = 0;
      fhCellClusterELam0BinPerSM        [il0][ism] = 0;
      fhCellClusterELam0BinPerSMWeighted[il0][ism] = 0;
      
      fhCellClusterEFracLam0BinPerSMLargeTime [il0][ism] = 0;
      fhCellClusterEFracLam0BinPerSMLargeTimeTotal[il0][ism] = 0;
      fhCellClusterELam0BinPerSMLargeTime     [il0][ism] = 0; 
      fhCellClusterIndexELam0BinPerSMLargeTime[il0][ism] = 0;
      fhNCellsWithLargeTimeInCluster          [il0][ism] = 0;
    }
  }
  
//  for(Int_t il0 = 0; il0 < 2; il0++)
//  {
//    for(Int_t i = 0; i < 7; i++) 
//    {
//      fhEtaPhiLam0BinPtBinSMShared      [il0][i] = 0 ;
//      fhColRowLam0BinPtBinSMShared      [il0][i] = 0 ;              
//    }
//    
//    for(Int_t ism =0; ism < 12; ism++)
//    {
//      fhTimeLam0BinPerSMShared[il0][ism] = 0;   
//      fhLam0PerSMShared            [ism] = 0;
//      fhLam1PerSMShared            [ism] = 0;
//    }
//  }
  
  for(Int_t ism = 0; ism < 20; ism++)
  {
    fhLam0PerSM                      [ism] = 0;
    fhLam1PerSM                      [ism] = 0;
    fhLam0PerSMLargeTimeInClusterCell[ism] = 0;
    fhLam1PerSMLargeTimeInClusterCell[ism] = 0;
//  fhLam0PerSMSPDPileUp             [ism] = 0;
//  fhLam1PerSMSPDPileUp             [ism] = 0; 
  }
  
  for(Int_t ilarge = 0; ilarge < 5; ilarge++)
  {
    fhLam0PerNLargeTimeInClusterCell[ilarge] = 0;
    fhLam1PerNLargeTimeInClusterCell[ilarge] = 0;
  }
  
  for(Int_t igen = 0; igen < 10; igen++)
  { 
    for(Int_t ip = 0; ip < fgkNGenTypes; ip++)
    {
      fhMergeGeneratorCluster                 [igen][ip] = 0; 
      fhMergeGeneratorClusterNotHijingBkg     [igen][ip] = 0;        
      fhMergeGeneratorClusterHijingAndOtherBkg[igen][ip] = 0;
      fhMergeGeneratorClusterHijingBkg        [igen][ip] = 0;  
      fhCleanGeneratorCluster                 [igen][ip] = 0;
      
      fhMergeGeneratorClusterEPrimRecoRatio                 [igen][ip] = 0;
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio     [igen][ip] = 0;        
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[igen][ip] = 0;
      fhMergeGeneratorClusterHijingBkgEPrimRecoRatio        [igen][ip] = 0;  
      fhCleanGeneratorClusterEPrimRecoRatio                 [igen][ip] = 0;  
      
      fhMergeGeneratorClusterEPrimRecoDiff                 [igen][ip] = 0; 
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff     [igen][ip] = 0;        
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[igen][ip] = 0;
      fhMergeGeneratorClusterHijingBkgEPrimRecoDiff        [igen][ip] = 0;  
      fhCleanGeneratorClusterEPrimRecoDiff                 [igen][ip] = 0;  
    }
  }
  
  for(Int_t icase = 0; icase < 6; icase++)
  { 
    fhLocalRegionClusterEtaPhi                         [icase] = 0; 
    fhLocalRegionClusterEnergySum                      [icase] = 0;     
    fhLocalRegionClusterMultiplicity                   [icase] = 0;
    fhLocalRegionClusterEnergySumPerCentrality         [icase] = 0;
    fhLocalRegionClusterMultiplicityPerCentrality      [icase] = 0;
    fhLocalRegionClusterEnergySumHijing                [icase] = 0;
    fhLocalRegionClusterMultiplicityHijing             [icase] = 0;
    fhLocalRegionClusterEnergySumPerCentralityHijing   [icase] = 0;
    fhLocalRegionClusterMultiplicityPerCentralityHijing[icase] = 0; 
    fhLocalRegionClusterEnergySumAdded                 [icase] = 0;     
    fhLocalRegionClusterMultiplicityAdded              [icase] = 0;
    fhLocalRegionClusterEnergySumPerCentralityAdded    [icase] = 0;
    fhLocalRegionClusterMultiplicityPerCentralityAdded [icase] = 0;
    
    fhLocalRegionClusterEnergySumMCPi0Decay                      [icase] = 0;     
    fhLocalRegionClusterMultiplicityMCPi0Decay                   [icase] = 0;
    fhLocalRegionClusterEnergySumPerCentralityMCPi0Decay         [icase] = 0;
    fhLocalRegionClusterMultiplicityPerCentralityMCPi0Decay      [icase] = 0;
    fhLocalRegionClusterEnergySumHijingMCPi0Decay                [icase] = 0;
    fhLocalRegionClusterMultiplicityHijingMCPi0Decay             [icase] = 0;
    fhLocalRegionClusterEnergySumPerCentralityHijingMCPi0Decay   [icase] = 0;
    fhLocalRegionClusterMultiplicityPerCentralityHijingMCPi0Decay[icase] = 0; 
    fhLocalRegionClusterEnergySumAddedMCPi0Decay                 [icase] = 0;     
    fhLocalRegionClusterMultiplicityAddedMCPi0Decay              [icase] = 0;
    fhLocalRegionClusterEnergySumPerCentralityAddedMCPi0Decay    [icase] = 0;
    fhLocalRegionClusterMultiplicityPerCentralityAddedMCPi0Decay [icase] = 0;
  }
  
  // Initialize parameters
  InitParameters();
}

//_____________________________________________________________________
/// Check the cluster activity, total energy and multiplicity around 
/// a selected cluster on EMCal acceptance within a cone of R=0.2
///
/// \param icalo: cluster index in array
/// \param en  : selected cluster energy
/// \param eta : cluster pseudo-rapidity
/// \param phi : cluster azimuthal angle (0-360 deg)
/// \param mctag: MC label tagß
/// \param clusterList: clusters array
//__________________________________________________________________________
void AliAnaPhoton::ActivityNearCluster(Int_t icalo, Float_t en, 
                                       Float_t eta, Float_t phi, 
                                       Int_t mctag, TObjArray *clusterList)
{
  Float_t radius = 0.2; // Hardcoded unless real use.
  
  // Accept cluster on EMCal limits - radius
  // CAREFUL if used for Run2 with DCal.
  if(phi < 3.15 - radius && phi > 1.4 + radius && TMath::Abs(eta) < 0.7-radius)
  {
    TString genName , genNameBkg ;
    Int_t   genIndex, genIndexBkg;
    Int_t genBkgTag = -1;
    
    TString genName2 , genNameBkg2 ;
    Int_t   genIndex2, genIndexBkg2;
    Int_t genBkgTag2 = -1;

    if ( IsDataMC() && IsStudyClusterOverlapsPerGeneratorOn() )
    {
      AliVCluster * calo = (AliVCluster*) (clusterList->At(icalo));
      
      genBkgTag = GetCocktailGeneratorBackgroundTag(calo, mctag, genName, genIndex, genNameBkg, genIndexBkg);
      
     }
    
    Bool_t mcDecay = kFALSE;
    if( IsDataMC() && !genName.Contains("ijing") &&
       GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCPi0Decay) && 
       !GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCPi0)        )  
      mcDecay = kTRUE;
    
    fhLocalRegionClusterEtaPhi[0] ->Fill(eta,phi, GetEventWeight());
    
    Float_t sumE   = 0;
    Int_t   sumM   = 0;
    Float_t sumEHi = 0;
    Int_t   sumMHi = 0;
    Float_t sumEHi2 = 0;
    Int_t   sumMHi2 = 0;

    for(Int_t icalo2 = 0; icalo2 < clusterList->GetEntriesFast(); icalo2++)
    {
      if ( icalo2 == icalo ) continue;
      
      AliVCluster * calo2 =  (AliVCluster*) (clusterList->At(icalo2));
      
      // Select clusters in a radius of R=0.2
      calo2->GetMomentum(fMomentum2,GetVertex(0)) ;
      
      Float_t dEta = eta-fMomentum2.Eta();
      Float_t dPhi = phi-GetPhi(fMomentum2.Phi());
      
      if(TMath::Abs(dPhi) >= TMath::Pi())
        dPhi = TMath::TwoPi()-TMath::Abs(dPhi);
            
      Float_t distance = TMath::Sqrt( dEta*dEta + dPhi*dPhi );
      
      genNameBkg2  = "";
      genName2     = "";
      genIndexBkg2 = -1;
      genIndex2    = -1;
      genBkgTag2   = -1;
      if ( IsDataMC() && IsStudyClusterOverlapsPerGeneratorOn() && calo2->GetNLabels() > 0 && distance < 0.4 )
      {
        genBkgTag2 = GetCocktailGeneratorBackgroundTag(calo2, -1, genName2, genIndex2, genNameBkg2, genIndexBkg2);

        if( !genName.Contains("ijing") && !genName2.Contains("ijing")) fhDistance2AddedSignals     ->Fill(en,distance,GetEventWeight());
        if( !genName.Contains("ijing") &&  genName2.Contains("ijing")) fhDistanceAddedSignalsHijing->Fill(en,distance,GetEventWeight());
        if(  genName.Contains("ijing") &&  genName2.Contains("ijing")) fhDistance2Hijing           ->Fill(en,distance,GetEventWeight());
      }
      
      if ( distance > radius) continue;
      
      sumM++;
      sumE += calo2->E();
      
      if ( IsDataMC() && IsStudyClusterOverlapsPerGeneratorOn() && calo2->GetNLabels() > 0)
      {        
        if(genName2.Contains("ijing"))
        {
          sumMHi++;
          sumEHi += calo2->E();
        }
      
        if(genName2.Contains("ijing") || genBkgTag2 == 1 || genBkgTag2 == 3)
        {
          sumMHi2++;
          sumEHi2 += calo2->E();
        }

      }
    }
    
    Float_t sumMAdd = sumM-sumMHi;
    Float_t sumEAdd = sumE-sumEHi;
        
    fhLocalRegionClusterEnergySum   [0]->Fill(en,sumE,GetEventWeight());
    fhLocalRegionClusterMultiplicity[0]->Fill(en,sumM,GetEventWeight());
    
    if(mcDecay)
    {
      fhLocalRegionClusterEnergySumMCPi0Decay   [0]->Fill(en,sumE,GetEventWeight());
      fhLocalRegionClusterMultiplicityMCPi0Decay[0]->Fill(en,sumM,GetEventWeight());
    }
    
    if(IsHighMultiplicityAnalysisOn())
    {
      fhLocalRegionClusterEnergySumPerCentrality   [0]->Fill(GetEventCentrality(),sumE,GetEventWeight());
      fhLocalRegionClusterMultiplicityPerCentrality[0]->Fill(GetEventCentrality(),sumM,GetEventWeight());
      
      if(mcDecay)
      {
        fhLocalRegionClusterEnergySumPerCentralityMCPi0Decay   [0]->Fill(GetEventCentrality(),sumE,GetEventWeight());
        fhLocalRegionClusterMultiplicityPerCentralityMCPi0Decay[0]->Fill(GetEventCentrality(),sumM,GetEventWeight());
      }
    }
    
    if( IsDataMC() && IsStudyClusterOverlapsPerGeneratorOn() )
    {
      fhLocalRegionClusterEnergySumHijing   [0]->Fill(en,sumEHi ,GetEventWeight());
      fhLocalRegionClusterMultiplicityHijing[0]->Fill(en,sumMHi ,GetEventWeight());

      fhLocalRegionClusterEnergySumHijing2   ->Fill(en,sumEHi2 ,GetEventWeight());
      fhLocalRegionClusterMultiplicityHijing2->Fill(en,sumMHi2 ,GetEventWeight());
      
      fhLocalRegionClusterEnergySumAdded    [0]->Fill(en,sumEAdd,GetEventWeight());
      fhLocalRegionClusterMultiplicityAdded [0]->Fill(en,sumMAdd,GetEventWeight());
      
      if(IsHighMultiplicityAnalysisOn())
      {
        fhLocalRegionClusterEnergySumPerCentralityHijing   [0]->Fill(GetEventCentrality(),sumEHi ,GetEventWeight());
        fhLocalRegionClusterMultiplicityPerCentralityHijing[0]->Fill(GetEventCentrality(),sumMHi ,GetEventWeight());        

        fhLocalRegionClusterEnergySumPerCentralityHijing2   ->Fill(GetEventCentrality(),sumEHi2 ,GetEventWeight());
        fhLocalRegionClusterMultiplicityPerCentralityHijing2->Fill(GetEventCentrality(),sumMHi2 ,GetEventWeight());    
        
        fhLocalRegionClusterEnergySumPerCentralityAdded    [0]->Fill(GetEventCentrality(),sumEAdd,GetEventWeight());
        fhLocalRegionClusterMultiplicityPerCentralityAdded [0]->Fill(GetEventCentrality(),sumMAdd,GetEventWeight());
      }
      
      if(mcDecay)
      {
        fhLocalRegionClusterEnergySumHijingMCPi0Decay   [0]->Fill(en,sumEHi ,GetEventWeight());
        fhLocalRegionClusterMultiplicityHijingMCPi0Decay[0]->Fill(en,sumMHi ,GetEventWeight());
        
        fhLocalRegionClusterEnergySumAddedMCPi0Decay    [0]->Fill(en,sumEAdd,GetEventWeight());
        fhLocalRegionClusterMultiplicityAddedMCPi0Decay [0]->Fill(en,sumMAdd,GetEventWeight());
        
        if(IsHighMultiplicityAnalysisOn())
        {
          fhLocalRegionClusterEnergySumPerCentralityHijingMCPi0Decay   [0]->Fill(GetEventCentrality(),sumEHi ,GetEventWeight());
          fhLocalRegionClusterMultiplicityPerCentralityHijingMCPi0Decay[0]->Fill(GetEventCentrality(),sumMHi ,GetEventWeight());        
          
          fhLocalRegionClusterEnergySumPerCentralityAddedMCPi0Decay    [0]->Fill(GetEventCentrality(),sumEAdd,GetEventWeight());
          fhLocalRegionClusterMultiplicityPerCentralityAddedMCPi0Decay [0]->Fill(GetEventCentrality(),sumMAdd,GetEventWeight());
        }
      }
      
      if(genBkgTag < 0) return;
      
      fhLocalRegionClusterEtaPhi            [genBkgTag+1] ->Fill(eta,phi, GetEventWeight());
      
      fhLocalRegionClusterEnergySum         [genBkgTag+1]->Fill(en,sumE,GetEventWeight());
      fhLocalRegionClusterMultiplicity      [genBkgTag+1]->Fill(en,sumM,GetEventWeight());
      
      fhLocalRegionClusterEnergySumHijing   [genBkgTag+1]->Fill(en,sumEHi ,GetEventWeight());
      fhLocalRegionClusterMultiplicityHijing[genBkgTag+1]->Fill(en,sumMHi ,GetEventWeight());
      
      fhLocalRegionClusterEnergySumAdded    [genBkgTag+1]->Fill(en,sumEAdd,GetEventWeight());
      fhLocalRegionClusterMultiplicityAdded [genBkgTag+1]->Fill(en,sumMAdd,GetEventWeight());
      
      if(IsHighMultiplicityAnalysisOn())
      {
        fhLocalRegionClusterEnergySumPerCentrality         [genBkgTag+1]->Fill(GetEventCentrality(),sumE,GetEventWeight());
        fhLocalRegionClusterMultiplicityPerCentrality      [genBkgTag+1]->Fill(GetEventCentrality(),sumM,GetEventWeight());
        
        fhLocalRegionClusterEnergySumPerCentralityHijing   [genBkgTag+1]->Fill(GetEventCentrality(),sumEHi ,GetEventWeight());
        fhLocalRegionClusterMultiplicityPerCentralityHijing[genBkgTag+1]->Fill(GetEventCentrality(),sumMHi ,GetEventWeight());        
        
        fhLocalRegionClusterEnergySumPerCentralityAdded    [genBkgTag+1]->Fill(GetEventCentrality(),sumEAdd,GetEventWeight());
        fhLocalRegionClusterMultiplicityPerCentralityAdded [genBkgTag+1]->Fill(GetEventCentrality(),sumMAdd,GetEventWeight());
      }
      
      if(mcDecay)
      {        
        fhLocalRegionClusterEnergySumMCPi0Decay         [genBkgTag+1]->Fill(en,sumE,GetEventWeight());
        fhLocalRegionClusterMultiplicityMCPi0Decay      [genBkgTag+1]->Fill(en,sumM,GetEventWeight());
        
        fhLocalRegionClusterEnergySumHijingMCPi0Decay   [genBkgTag+1]->Fill(en,sumEHi ,GetEventWeight());
        fhLocalRegionClusterMultiplicityHijingMCPi0Decay[genBkgTag+1]->Fill(en,sumMHi ,GetEventWeight());
        
        fhLocalRegionClusterEnergySumAddedMCPi0Decay    [genBkgTag+1]->Fill(en,sumEAdd,GetEventWeight());
        fhLocalRegionClusterMultiplicityAddedMCPi0Decay [genBkgTag+1]->Fill(en,sumMAdd,GetEventWeight());
        
        if(IsHighMultiplicityAnalysisOn())
        {
          fhLocalRegionClusterEnergySumPerCentralityMCPi0Decay         [genBkgTag+1]->Fill(GetEventCentrality(),sumE,GetEventWeight());
          fhLocalRegionClusterMultiplicityPerCentralityMCPi0Decay      [genBkgTag+1]->Fill(GetEventCentrality(),sumM,GetEventWeight());
          
          fhLocalRegionClusterEnergySumPerCentralityHijingMCPi0Decay   [genBkgTag+1]->Fill(GetEventCentrality(),sumEHi ,GetEventWeight());
          fhLocalRegionClusterMultiplicityPerCentralityHijingMCPi0Decay[genBkgTag+1]->Fill(GetEventCentrality(),sumMHi ,GetEventWeight());        
          
          fhLocalRegionClusterEnergySumPerCentralityAddedMCPi0Decay    [genBkgTag+1]->Fill(GetEventCentrality(),sumEAdd,GetEventWeight());
          fhLocalRegionClusterMultiplicityPerCentralityAddedMCPi0Decay [genBkgTag+1]->Fill(GetEventCentrality(),sumMAdd,GetEventWeight());
        }
      }
      
      if(genBkgTag > 0) // Not clean, all merged
      {
        fhLocalRegionClusterEtaPhi            [5] ->Fill(eta,phi, GetEventWeight());
        
        fhLocalRegionClusterEnergySum         [5]->Fill(en,sumE,GetEventWeight());
        fhLocalRegionClusterMultiplicity      [5]->Fill(en,sumM,GetEventWeight());
        
        fhLocalRegionClusterEnergySumHijing   [5]->Fill(en,sumEHi ,GetEventWeight());
        fhLocalRegionClusterMultiplicityHijing[5]->Fill(en,sumMHi ,GetEventWeight());
        
        fhLocalRegionClusterEnergySumAdded    [5]->Fill(en,sumEAdd,GetEventWeight());
        fhLocalRegionClusterMultiplicityAdded [5]->Fill(en,sumMAdd,GetEventWeight());
        
        if(IsHighMultiplicityAnalysisOn())
        {
          fhLocalRegionClusterEnergySumPerCentrality         [5]->Fill(GetEventCentrality(),sumE,GetEventWeight());
          fhLocalRegionClusterMultiplicityPerCentrality      [5]->Fill(GetEventCentrality(),sumM,GetEventWeight());
          
          fhLocalRegionClusterEnergySumPerCentralityHijing   [5]->Fill(GetEventCentrality(),sumEHi ,GetEventWeight());
          fhLocalRegionClusterMultiplicityPerCentralityHijing[5]->Fill(GetEventCentrality(),sumMHi ,GetEventWeight());        
          
          fhLocalRegionClusterEnergySumPerCentralityAdded    [5]->Fill(GetEventCentrality(),sumEAdd,GetEventWeight());
          fhLocalRegionClusterMultiplicityPerCentralityAdded [5]->Fill(GetEventCentrality(),sumMAdd,GetEventWeight());
        }

        if(mcDecay)
        {          
          fhLocalRegionClusterEnergySumMCPi0Decay         [5]->Fill(en,sumE,GetEventWeight());
          fhLocalRegionClusterMultiplicityMCPi0Decay      [5]->Fill(en,sumM,GetEventWeight());
          
          fhLocalRegionClusterEnergySumHijingMCPi0Decay   [5]->Fill(en,sumEHi ,GetEventWeight());
          fhLocalRegionClusterMultiplicityHijingMCPi0Decay[5]->Fill(en,sumMHi ,GetEventWeight());
          
          fhLocalRegionClusterEnergySumAddedMCPi0Decay    [5]->Fill(en,sumEAdd,GetEventWeight());
          fhLocalRegionClusterMultiplicityAddedMCPi0Decay [5]->Fill(en,sumMAdd,GetEventWeight());
          
          if(IsHighMultiplicityAnalysisOn())
          {
            fhLocalRegionClusterEnergySumPerCentralityMCPi0Decay         [5]->Fill(GetEventCentrality(),sumE,GetEventWeight());
            fhLocalRegionClusterMultiplicityPerCentralityMCPi0Decay      [5]->Fill(GetEventCentrality(),sumM,GetEventWeight());
            
            fhLocalRegionClusterEnergySumPerCentralityHijingMCPi0Decay   [5]->Fill(GetEventCentrality(),sumEHi ,GetEventWeight());
            fhLocalRegionClusterMultiplicityPerCentralityHijingMCPi0Decay[5]->Fill(GetEventCentrality(),sumMHi ,GetEventWeight());        
            
            fhLocalRegionClusterEnergySumPerCentralityAddedMCPi0Decay    [5]->Fill(GetEventCentrality(),sumEAdd,GetEventWeight());
            fhLocalRegionClusterMultiplicityPerCentralityAddedMCPi0Decay [5]->Fill(GetEventCentrality(),sumMAdd,GetEventWeight());
          }
        }
        
      } // not clean, all merged
      
    } // check generator overlaps
    
  } // EMCal acceptance
}

//_____________________________________________________________________
/// Check the particle overlaps into a cluster when originated by 
/// different generators
///
/// \param calo: cluster pointer
/// \param mctag  : mc tag label, of the originating particle
//__________________________________________________________________________
void AliAnaPhoton::CocktailGeneratorsClusterOverlaps(AliVCluster* calo, Int_t mctag)
{
  //
  // Check the generators inside the cluster
  TString genName  = "", genNameBkg  = "";
  Int_t   genIndex = -1, genIndexBkg = -1;
  Int_t genBkgTag = GetCocktailGeneratorBackgroundTag(calo, mctag, genName, genIndex, genNameBkg, genIndexBkg);
  if     (genBkgTag == -1) return;
  else if(genBkgTag  >  3) printf("Bkg generator tag larger than 3\n");
  
  //
  // Get primary particle info of main particle contributing to the cluster
  Float_t eprim   = 0;
  //Float_t ptprim  = 0;
  Bool_t ok = kFALSE;
  Int_t pdg = 0, status = 0, momLabel = -1;
  
  //fPrimaryMom = GetMCAnalysisUtils()->GetMother(label,GetReader(),ok);
  fPrimaryMom = GetMCAnalysisUtils()->GetMother(calo->GetLabel(),GetMC(), pdg, status, ok, momLabel);     
  
  if(ok)
  {
    eprim   = fPrimaryMom.Energy();
    //ptprim  = fPrimaryMom.Pt();
  }
  
  //
  Float_t en = calo->E();

  if ( eprim < 0.1 || en < 0.5 ) return;
  
  //
  Int_t partType = -1;
  if     ( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCPi0)      ) partType = kmcGenPi0Merged;
  else if( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCPi0Decay) ) partType = kmcGenPi0Decay;
  else if( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCEtaDecay) ) partType = kmcGenEtaDecay;
  else if( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCPhoton)   ) partType = kmcGenPhoton;
  else if( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCElectron) ) partType = kmcGenElectron;
  else                                                                                partType = kmcGenOther;

  Int_t genType = GetNCocktailGenNamesToCheck()-1;
  for(Int_t igen = 1; igen < GetNCocktailGenNamesToCheck(); igen++)
  {
    if ( GetCocktailGenNameToCheck(igen).Contains(genName) && 
        ( GetCocktailGenIndexToCheck(igen) < 0 || genIndex == GetCocktailGenIndexToCheck(igen)) )
    {
      genType = igen;
      break;
    }
  }
  
  Float_t ratio = en / eprim;
  Float_t diff  = en - eprim;
    
  if ( genBkgTag > 0 ) 
  {
    fhMergeGeneratorCluster[0]      [0]        ->Fill(en,GetEventWeight());
    fhMergeGeneratorCluster[0]      [partType] ->Fill(en,GetEventWeight());
    fhMergeGeneratorCluster[genType][0]        ->Fill(en,GetEventWeight());
    fhMergeGeneratorCluster[genType][partType] ->Fill(en,GetEventWeight());

    fhMergeGeneratorClusterEPrimRecoRatio[0]      [0]        ->Fill(en,ratio,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoRatio[0]      [partType] ->Fill(en,ratio,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoRatio[genType][0]        ->Fill(en,ratio,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoRatio[genType][partType] ->Fill(en,ratio,GetEventWeight());
    
    fhMergeGeneratorClusterEPrimRecoDiff[0]      [0]        ->Fill(en,diff,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoDiff[0]      [partType] ->Fill(en,diff,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoDiff[genType][0]        ->Fill(en,diff,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoDiff[genType][partType] ->Fill(en,diff,GetEventWeight());
    
    if(genBkgTag == 2) 
    {
      fhMergeGeneratorClusterNotHijingBkg[0]      [0]        ->Fill(en,GetEventWeight());
      fhMergeGeneratorClusterNotHijingBkg[0]      [partType] ->Fill(en,GetEventWeight());
      fhMergeGeneratorClusterNotHijingBkg[genType][0]        ->Fill(en,GetEventWeight());
      fhMergeGeneratorClusterNotHijingBkg[genType][partType] ->Fill(en,GetEventWeight());
      
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[0]      [0]        ->Fill(en,ratio,GetEventWeight());
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[0]      [partType] ->Fill(en,ratio,GetEventWeight());
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[genType][0]        ->Fill(en,ratio,GetEventWeight());
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[genType][partType] ->Fill(en,ratio,GetEventWeight());
      
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[0]      [0]        ->Fill(en,diff,GetEventWeight());
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[0]      [partType] ->Fill(en,diff,GetEventWeight());
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[genType][0]        ->Fill(en,diff,GetEventWeight());
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[genType][partType] ->Fill(en,diff,GetEventWeight());
    }
    
    if(genBkgTag == 1)
    {
      fhMergeGeneratorClusterHijingBkg[0]      [0]        ->Fill(en,GetEventWeight());
      fhMergeGeneratorClusterHijingBkg[0]      [partType] ->Fill(en,GetEventWeight());
      fhMergeGeneratorClusterHijingBkg[genType][0]        ->Fill(en,GetEventWeight());
      fhMergeGeneratorClusterHijingBkg[genType][partType] ->Fill(en,GetEventWeight());
      
      fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[0]      [0]        ->Fill(en,ratio,GetEventWeight());
      fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[0]      [partType] ->Fill(en,ratio,GetEventWeight());
      fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[genType][0]        ->Fill(en,ratio,GetEventWeight());
      fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[genType][partType] ->Fill(en,ratio,GetEventWeight());
      
      fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[0]      [0]        ->Fill(en,diff,GetEventWeight());
      fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[0]      [partType] ->Fill(en,diff,GetEventWeight());
      fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[genType][0]        ->Fill(en,diff,GetEventWeight());
      fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[genType][partType] ->Fill(en,diff,GetEventWeight());
    }
    
    if ( genBkgTag == 3 )
    {      
      fhMergeGeneratorClusterHijingAndOtherBkg[0]      [0]        ->Fill(en,GetEventWeight());
      fhMergeGeneratorClusterHijingAndOtherBkg[0]      [partType] ->Fill(en,GetEventWeight());
      fhMergeGeneratorClusterHijingAndOtherBkg[genType][0]        ->Fill(en,GetEventWeight());
      fhMergeGeneratorClusterHijingAndOtherBkg[genType][partType] ->Fill(en,GetEventWeight());
      
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[0]      [0]        ->Fill(en,ratio,GetEventWeight());
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[0]      [partType] ->Fill(en,ratio,GetEventWeight());
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[genType][0]        ->Fill(en,ratio,GetEventWeight());
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[genType][partType] ->Fill(en,ratio,GetEventWeight());
      
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[0]      [0]        ->Fill(en,diff,GetEventWeight());
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[0]      [partType] ->Fill(en,diff,GetEventWeight());
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[genType][0]        ->Fill(en,diff,GetEventWeight());
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[genType][partType] ->Fill(en,diff,GetEventWeight());
    }
  }
  else    
  {
    fhCleanGeneratorCluster[0]      [0]        ->Fill(en,GetEventWeight());
    fhCleanGeneratorCluster[0]      [partType] ->Fill(en,GetEventWeight());
    fhCleanGeneratorCluster[genType][0]        ->Fill(en,GetEventWeight());
    fhCleanGeneratorCluster[genType][partType] ->Fill(en,GetEventWeight());
    
    fhCleanGeneratorClusterEPrimRecoRatio[0]      [0]        ->Fill(en,ratio,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoRatio[0]      [partType] ->Fill(en,ratio,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoRatio[genType][0]        ->Fill(en,ratio,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoRatio[genType][partType] ->Fill(en,ratio,GetEventWeight());
    
    fhCleanGeneratorClusterEPrimRecoDiff[0]      [0]        ->Fill(en,diff,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoDiff[0]      [partType] ->Fill(en,diff,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoDiff[genType][0]        ->Fill(en,diff,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoDiff[genType][partType] ->Fill(en,diff,GetEventWeight());
  }
}


//_____________________________________________________________________
/// Select calorimeter clusters if they pass different cuts:
///  * Energy (if stricter cut than in AliCaloTrackReader)
///  * Time (but usually it is already done in AliCaloTrackReader)
///  * Number of cells in cluster
///  * Number of local maxima in cluster
///  * Fiducial cut, eta-phi acceptance cut via AliFiducialCut
///  * Charged clusters are rejected (if requested)
///  * Reject clusters close to a bad channel
///
/// Fill for each of the cuts a 1 dimensional histogram with either the energy
/// or the transverse momentum of the cluster. Also track-matching control histograms
/// can be filled with residuals of the matching.
/// \return kTRUE of cluster is accepted
/// \param calo    : cluster pointer.
/// \param sm      : cluster super module number.
/// \param nMaxima : number of local maxima.
/// \param matched : is cluster matched to a track
/// \param bEoP: If rejection is due to E over P cut, set it true, else false
/// \param bRes: If rejection is due to residual eta-phi cut, set it true, else false
/// \param mctag   : tag containing MC origin of cluster.
/// \param mcbin   : particle tag for MC histogram
/// \param egen    : main MC particle generated energy
/// \param noverlaps: number of extra MC particles contributing 
/// \param weightPt: histogram weight depending on particle generated
//_____________________________________________________________________
Bool_t  AliAnaPhoton::ClusterSelected(AliVCluster* calo, Int_t sm, Int_t nMaxima, 
                                      Bool_t matched, Bool_t bEoP, Bool_t bRes,
                                      Int_t mctag, Float_t mcbin, Float_t egen, 
                                      Int_t noverlaps, Float_t weightPt)
{
  Float_t ptcluster  = fMomentum.Pt();
  Float_t ecluster   = fMomentum.E();
  Float_t etacluster = fMomentum.Eta();
  Float_t phicluster = fMomentum.Phi();

  if ( phicluster < 0 ) phicluster+=TMath::TwoPi();

  AliDebug(2,Form("Current Event %d; Before selection : E %2.2f, pT %2.2f, phi %2.2f, eta %2.2f",
           GetReader()->GetEventNumber(),
           ecluster, ptcluster, phicluster*TMath::RadToDeg(), etacluster));
    
  fhClusterCutsE [1]->Fill( ecluster, GetEventWeight()*weightPt);
  fhClusterCutsPt[1]->Fill(ptcluster, GetEventWeight()*weightPt);
  
  if ( ecluster > 0.5 ) fhEtaPhi->Fill(etacluster, phicluster, GetEventWeight()*weightPt);
  
  if ( sm < fFirstModule || sm > fLastModule  || sm >= fNModules || sm < 0 ) 
  {
    AliDebug(1,Form("Reject, SM %d out of selected range [%d, %d]",
                    sm, fFirstModule, fLastModule));
    return kFALSE;
  }
  
  if ( !IsHighMultiplicityAnalysisOn() )
  {
    if ( !fFillOnlyPtHisto )
      fhEClusterSM ->Fill(ecluster , sm, GetEventWeight()*weightPt);
    fhPtClusterSM->Fill(ptcluster, sm, GetEventWeight()*weightPt);
  }
  
  //.......................................
  // If too small or big energy, skip it
  if ( ecluster < GetMinEnergy() || ecluster > GetMaxEnergy() ) return kFALSE ;
  
  AliDebug(2,Form("\t Cluster %d Pass E Cut",calo->GetID()));
  
  fhClusterCutsE [2]->Fill( ecluster, GetEventWeight()*weightPt);
  fhClusterCutsPt[2]->Fill(ptcluster, GetEventWeight()*weightPt);
  
  //.......................................
  // TOF cut, BE CAREFUL WITH THIS CUT
  Double_t tof = calo->GetTOF()*1e9;
  if ( tof > 400 ) tof-=fConstantTimeShift; // only for MC, rest shift = 0
  
  if ( tof < fTimeCutMin || tof > fTimeCutMax ) return kFALSE;
  
  AliDebug(2,Form("\t Cluster %d Pass Time Cut",calo->GetID()));
  
  fhClusterCutsE [3]->Fill( ecluster, GetEventWeight()*weightPt);
  fhClusterCutsPt[3]->Fill(ptcluster, GetEventWeight()*weightPt);
  
  //.......................................
  
  if ( fFillControlClusterContentHisto )
  {
    if ( !IsHighMultiplicityAnalysisOn() )
    {
      fhNCellsECluster ->Fill(ecluster, calo->GetNCells(), GetEventWeight()*weightPt);
      fhNLocMaxECluster->Fill(ecluster, nMaxima          , GetEventWeight()*weightPt);
      fhLam0ECluster   ->Fill(ecluster, calo->GetM02()   , GetEventWeight()*weightPt);
      
      if ( fFillSSNLocMaxHisto )   
        fhLam0NLocMaxECluster->Fill(ecluster, calo->GetM02(), nMaxima, GetEventWeight()*weightPt);
      
      if ( !matched && fRejectTrackMatch )
      {
        fhNCellsEClusterNeutral ->Fill(ecluster, calo->GetNCells(), GetEventWeight()*weightPt);
        fhNLocMaxEClusterNeutral->Fill(ecluster, nMaxima          , GetEventWeight()*weightPt);
        fhLam0EClusterNeutral   ->Fill(ecluster, calo->GetM02()   , GetEventWeight()*weightPt);
        
        if ( fFillSSNLocMaxHisto )   
          fhLam0NLocMaxEClusterNeutral->Fill(ecluster, calo->GetM02(), nMaxima, GetEventWeight()*weightPt);
      }
    }
    else
    {
      Float_t cen = GetEventCentrality();
      
      fhNCellsCentralityECluster ->Fill(ecluster, calo->GetNCells(), cen, GetEventWeight()*weightPt);
      fhNLocMaxCentralityECluster->Fill(ecluster, nMaxima          , cen, GetEventWeight()*weightPt);
      fhLam0CentralityECluster   ->Fill(ecluster, calo->GetM02()   , cen, GetEventWeight()*weightPt);

      Int_t icent = GetEventCentralityBin();
      if ( fFillSSNLocMaxHisto && 
          icent >= 0 && GetNCentrBin() > 0 && icent < GetNCentrBin() && icent < GetNCentrBin() )   
        fhLam0NLocMaxEClusterPerCen[icent]->Fill(ecluster, calo->GetM02(), nMaxima, GetEventWeight()*weightPt);

      if ( !matched && fRejectTrackMatch )
      {
        fhNCellsCentralityEClusterNeutral ->Fill(ecluster, calo->GetNCells(), cen, GetEventWeight()*weightPt);
        fhNLocMaxCentralityEClusterNeutral->Fill(ecluster, nMaxima          , cen, GetEventWeight()*weightPt);
        fhLam0CentralityEClusterNeutral   ->Fill(ecluster, calo->GetM02()   , cen, GetEventWeight()*weightPt);

        if ( fFillSSNLocMaxHisto  && 
            icent >= 0 && GetNCentrBin() > 0 && icent < GetNCentrBin() ) 
          fhLam0NLocMaxEClusterNeutralPerCen[icent]->Fill(ecluster, calo->GetM02(), nMaxima, GetEventWeight()*weightPt);
      }

    }
  } // shape control histograms
  
  if ( calo->GetNCells() <= fNCellsCut && 
       GetReader()->GetDataType() != AliCaloTrackReader::kMC) return kFALSE;
  
  AliDebug(2,Form("\t Cluster %d Pass NCell Cut",calo->GetID()));
  
  fhClusterCutsE [4]->Fill( ecluster, GetEventWeight()*weightPt);
  fhClusterCutsPt[4]->Fill(ptcluster, GetEventWeight()*weightPt);
  
  if ( nMaxima < fNLMCutMin || nMaxima > fNLMCutMax ) return kFALSE ;
  AliDebug(2,Form("\t Cluster %d pass NLM %d of out of range",calo->GetID(), nMaxima));
  
  fhClusterCutsE [5]->Fill( ecluster, GetEventWeight()*weightPt);
  fhClusterCutsPt[5]->Fill(ptcluster, GetEventWeight()*weightPt);
  
  //.......................................
  //Check acceptance selection
  if ( IsFiducialCutOn() )
  {
    Bool_t in = GetFiducialCut()->IsInFiducialCut(fMomentum.Eta(),fMomentum.Phi(),GetCalorimeter()) ;
    if ( !in ) return kFALSE ;
  }
  
  AliDebug(2,Form("\t Fiducial cut passed"));
  
  fhClusterCutsE [6]->Fill( ecluster, GetEventWeight()*weightPt);
  fhClusterCutsPt[6]->Fill(ptcluster, GetEventWeight()*weightPt);
  
  //.......................................
  // Skip matched clusters with tracks
  
  if ( IsDataMC() )
  {
    // Fill selected cluster histograms in MC before TM
    fhMCParticle[1]->Fill(ecluster, mcbin, GetEventWeight()*weightPt);
    
    if ( egen > 0.1 )
      fhMCParticleVsErecEgen[1]->Fill(ecluster, mcbin, ecluster/egen, GetEventWeight()*weightPt);
    
    if  ( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCConversion) )
      fhMCParticleConverted[1]->Fill(ecluster, mcbin, GetEventWeight()*weightPt);
    
    if ( fCheckOverlaps )
      fhMCParticleVsNOverlaps[1]->Fill(ecluster, mcbin, noverlaps, GetEventWeight()*weightPt);
  }
  
  // Fill matching residual histograms before PID cuts
  if ( fFillTMHisto ) 
    FillTrackMatchingResidualHistograms(calo, 0, sm, matched, bEoP, bRes, 
                                        mctag, mcbin, egen, noverlaps, weightPt);
  
  if ( fRejectTrackMatch )
  {
    if ( matched )
    {
      AliDebug(2,"\t Reject track-matched clusters");
      return kFALSE ;
    }
    else
      AliDebug(2,"\t Track-matching cut passed");
  }// reject matched clusters
  
  fhClusterCutsE [7]->Fill( ecluster, GetEventWeight()*weightPt);
  fhClusterCutsPt[7]->Fill(ptcluster, GetEventWeight()*weightPt);
  
  if ( IsDataMC() )
  {
    // Fill selected cluster histograms in MC before TM
    fhMCParticle[2]->Fill(ecluster, mcbin, GetEventWeight()*weightPt);
    
    if ( egen > 0.1 )
      fhMCParticleVsErecEgen[2]->Fill(ecluster, mcbin, ecluster/egen, GetEventWeight()*weightPt);
    
    if  ( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCConversion) )
      fhMCParticleConverted[2]->Fill(ecluster, mcbin, GetEventWeight()*weightPt);
    
    if ( fCheckOverlaps )
      fhMCParticleVsNOverlaps[2]->Fill(ecluster, mcbin, noverlaps, GetEventWeight()*weightPt);
  }
  
  //.......................................
  //Check Distance to Bad channel, set bit.
  Double_t distBad=calo->GetDistanceToBadChannel() ; //Distance to bad channel
  if ( distBad < 0.) distBad=9999. ; //workout strange convension dist = -1. ;
  if ( distBad < fMinDist )
  {
    //In bad channel (PHOS cristal size 2.2x2.2 cm), EMCAL ( cell units )
    return kFALSE ;
  }
  else AliDebug(2,Form("\t Bad channel cut passed %4.2f > %2.2f",distBad, fMinDist));
  
  fhClusterCutsE [8]->Fill( ecluster, GetEventWeight()*weightPt);
  fhClusterCutsPt[8]->Fill(ptcluster, GetEventWeight()*weightPt);
  
  AliDebug(1,Form("Current Event %d; After  selection : E %2.2f, pT %2.2f, phi %2.2f, eta %2.2f",
           GetReader()->GetEventNumber(),
           ecluster, ptcluster,fMomentum.Phi()*TMath::RadToDeg(),fMomentum.Eta()));
  
  //All checks passed, cluster selected
  return kTRUE;
  
}

//___________________________________________
/// Fill primary generated MC particles acceptance
/// histograms if MC data is available. Pure generated kinematics analysis.
/// Fill histograms for different photon particle types,
/// index defined in enum mcPTypes.
//___________________________________________
void AliAnaPhoton::FillAcceptanceHistograms()
{
  if( !GetMC() )
  {
    AliFatal("MCEvent not available, is the MC handler called? STOP");
    return;
  }
  
  Double_t photonY   = -100 ;
  Double_t photonE   = -1 ;
  Double_t photonPt  = -1 ;
  Double_t photonPhi =  100 ;
  Double_t photonEta = -1 ;
  
  Int_t    pdg       =  0 ;
  Int_t    tag       =  0 ;
  Int_t    status    =  0 ;
  Int_t    mcIndex   =  0 ;
  Int_t    nprim     =  GetMC()->GetNumberOfTracks();
  Bool_t   inacceptance = kFALSE ;
  
  Bool_t   ok        = kFALSE;
  Int_t    momLabel  = -1;
  
  TString genName    = "";

  AliVParticle * primary = 0;
  
  Int_t firstParticle = 0;
  
  // Loop only over likely final particles not partons
  if ( GetReader()->GetGenPythiaEventHeader() ) 
    firstParticle = GetMCAnalysisUtils()->GetPythiaMaxPartParent();
  
  for(Int_t i = firstParticle ; i < nprim; i++)
  {
    if ( !GetReader()->AcceptParticleMCLabel( i ) ) continue ;
    
    primary = GetMC()->GetTrack(i);
    if(!primary)
    {
      AliWarning("primaries pointer not available!!");
      continue;
    }
    
    pdg    = primary->PdgCode();
    status = primary->MCStatusCode();
    
    // Protection against floating point exception
    if ( primary->E() == TMath::Abs(primary->Pz()) || 
        (primary->E() - primary->Pz()) < 1e-3      ||
        (primary->E() + primary->Pz()) < 0           )  continue ; 
    
    //printf("i %d, %s %d  %s %d \n",i, GetMC()->Particle(i)->GetName(), GetMC()->Particle(i)->GetPdgCode(),
    //       prim->GetName(), prim->GetPdgCode());
    
    // Photon kinematics
    primary->Momentum(fMomentum);
    
    photonY = 0.5*TMath::Log((primary->E()+primary->Pz())/(primary->E()-primary->Pz())) ;
    
    //printf("particle %d, pdg %d, status %d, phys prim %d, pT %2.2f\n",i,pdg,status,primary->IsPhysicalPrimary(),primary->Pt());
    
    // Select only photons in the final state
    if(pdg != 22 ) continue ;
    
    // If too small or too large pt, skip, same cut as for data analysis
    photonPt  = fMomentum.Pt () ;
    
    if(photonPt < GetMinPt() || photonPt > GetMaxPt() ) continue ;
    
    photonE   = fMomentum.E  () ;
    photonEta = fMomentum.Eta() ;
    photonPhi = fMomentum.Phi() ;
    
    if(photonPhi < 0) photonPhi+=TMath::TwoPi();
    
    // Check if photons hit desired acceptance
    inacceptance = kTRUE;
    
    // Check same fidutial borders as in data analysis on top of real acceptance if real was requested.
    if( IsFiducialCutOn() && !GetFiducialCut()->IsInFiducialCut(fMomentum.Eta(),fMomentum.Phi(),GetCalorimeter())) inacceptance = kFALSE ;
    
    // Check if photons hit the Calorimeter acceptance
    if(IsRealCaloAcceptanceOn()) // defined on base class
    {
      if(!GetCaloUtils()->IsMCParticleInCalorimeterAcceptance(GetCalorimeter(), primary)) inacceptance = kFALSE ;
    }
  
    // Get tag of this particle photon from fragmentation, decay, prompt ...
    // Set the origin of the photon.
    tag = GetMCAnalysisUtils()->CheckOrigin(i, GetMC(),
                                            GetReader()->GetNameOfMCEventHederGeneratorToAccept(),
                                            photonE); // not needed
    
    if(!GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPhoton))
    {
      // A conversion photon from a hadron, skip this kind of photon
      // printf("AliAnaPhoton::FillAcceptanceHistograms() - not a photon, weird!\n ");
      // GetMCAnalysisUtils()->PrintMCTag(tag);
      
      continue;
    }
  
    //if(fStudyActivityNearCluster  && photonE > 5) // cut at 5 GeV to avoid too many iterations
     // DistanceToAddedSignalAtGeneratorLevel(i,nprim, GetMC(), photonE,photonEta,photonPhi);
    
    // Consider only final state particles, but this depends on generator,
    // status 1 is the usual one, in case of not being ok, leave the possibility
    // to not consider this.
    if(status > 1) continue ; // Avoid "partonic" photons
    
    /// Particle ID and pT dependent Weight
    Int_t   index    = GetReader()->GetCocktailGeneratorAndIndex(i, genName);
    Float_t weightPt = GetParticlePtWeight(photonPt, pdg, genName, index) ; 
    ///
    
    Bool_t takeIt  = kFALSE ;
    if(status == 1 && GetMCAnalysisUtils()->GetMCGenerator() != AliMCAnalysisUtils::kBoxLike ) takeIt = kTRUE ;

    if     (GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCConversion)) continue;
    
    // Origin of photon
    if     (GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPrompt))
    {
      mcIndex = kmcPPrompt;
    }
    else if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCFragmentation))
    {
      mcIndex = kmcPFragmentation ;
    }
    else if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCISR))
    {
      mcIndex = kmcPISR;
    }
    else if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0Decay))
    {
      mcIndex = kmcPPi0Decay;
      fPrimaryMom2 = GetMCAnalysisUtils()->GetMotherWithPDG(i, 111, GetMC(),ok, momLabel);        
      weightPt     = GetParticlePtWeight(fPrimaryMom2.Pt(), 111, genName, index) ; 
    }
    else if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEtaDecay))
    {
      mcIndex = kmcPEtaDecay;
      fPrimaryMom2 = GetMCAnalysisUtils()->GetMotherWithPDG(i, 221, GetMC(),ok, momLabel);        
      weightPt     = GetParticlePtWeight(fPrimaryMom2.Pt(), 221, genName, index) ; 
    }
    else if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCOtherDecay))
    {
      mcIndex = kmcPOtherDecay;
    }
    else
    {
      // Other decay but from non final state particle
      mcIndex = kmcPOtherDecay;
    } // Other origin
    
    if ( !takeIt &&  (mcIndex == kmcPPi0Decay || mcIndex == kmcPOtherDecay) ) takeIt = kTRUE ;

    if ( !takeIt ) continue ;
    
    // Fill rapidity histograms
    fhYPrimMC[kmcPPhoton]->Fill(photonPt, photonY, GetEventWeight()*weightPt) ;
    if ( mcIndex < fNPrimaryHistograms )
         fhYPrimMC[mcIndex]->Fill(photonPt, photonY, GetEventWeight()*weightPt) ;
    
    // Not interested in large rapidity, cut those
    if ( TMath::Abs(photonY) > 1.0 ) continue;
    
    // Avoid too large pT particle in pT hard binned productions
    if ( GetReader()->IsPythiaEventHeaderUsed() && GetReader()->GetGenPythiaEventHeader() ) 
    {
      Float_t pTHard = (GetReader()->GetGenPythiaEventHeader())->GetPtHard();
      
      if ( pTHard / photonPt < 0.5 ) 
      {
        AliInfo(Form("Reject primary particle pdg %d mcTag %d, pT %f larger than pT hard %f, ratio %f",
                     pdg, mcIndex, photonPt, pTHard, pTHard/photonPt));
    
        //GetMCAnalysisUtils()->PrintAncestry(GetMC(),i);
        continue;
      }
    }
    
    // Fill histograms for all photons
    //
    if ( !fFillOnlyPtHisto )
      fhEPrimMC  [kmcPPhoton]->Fill(photonE , GetEventWeight()*weightPt) ;
    fhPtPrimMC [kmcPPhoton]->Fill(photonPt, GetEventWeight()*weightPt) ;
    fhPhiPrimMC[kmcPPhoton]->Fill(photonPt , photonPhi, GetEventWeight()*weightPt) ;
    fhEtaPrimMC[kmcPPhoton]->Fill(photonPt , photonEta, GetEventWeight()*weightPt) ;
    
    if ( inacceptance )
    {
      if ( !fFillOnlyPtHisto )
        fhEPrimMCAcc  [kmcPPhoton]->Fill(photonE , GetEventWeight()*weightPt) ;
      fhPtPrimMCAcc [kmcPPhoton]->Fill(photonPt, GetEventWeight()*weightPt) ;
      fhPhiPrimMCAcc[kmcPPhoton]->Fill(photonPt , photonPhi, GetEventWeight()*weightPt) ;
      fhEtaPrimMCAcc[kmcPPhoton]->Fill(photonPt , photonEta, GetEventWeight()*weightPt) ;
      fhYPrimMCAcc  [kmcPPhoton]->Fill(photonPt , photonY  , GetEventWeight()*weightPt) ;
    } // Accepted
    
    // Fill histograms for photons origin
    //
    if ( mcIndex < fNPrimaryHistograms )
    {
      if ( !fFillOnlyPtHisto )
        fhEPrimMC  [mcIndex]->Fill(photonE , GetEventWeight()*weightPt) ;
      fhPtPrimMC [mcIndex]->Fill(photonPt, GetEventWeight()*weightPt) ;
      fhPhiPrimMC[mcIndex]->Fill(photonPt , photonPhi, GetEventWeight()*weightPt) ;
      fhEtaPrimMC[mcIndex]->Fill(photonPt , photonEta, GetEventWeight()*weightPt) ;
      
      if ( inacceptance )
      {
        if ( !fFillOnlyPtHisto )
          fhEPrimMCAcc  [mcIndex]->Fill(photonE , GetEventWeight()*weightPt) ;
        fhPtPrimMCAcc [mcIndex]->Fill(photonPt, GetEventWeight()*weightPt) ;
        fhPhiPrimMCAcc[mcIndex]->Fill(photonPt , photonPhi, GetEventWeight()*weightPt) ;
        fhEtaPrimMCAcc[mcIndex]->Fill(photonPt , photonEta, GetEventWeight()*weightPt) ;
        fhYPrimMCAcc  [mcIndex]->Fill(photonPt , photonY  , GetEventWeight()*weightPt) ;
      } // Accepted
    }
  } // loop on primaries
}

//________________________________________________________________________________
/// Calculate distance between a generated photon and a particle from an added
/// cocktail generator. Fill histograms.
///
///
//________________________________________________________________________________
//void AliAnaPhoton::DistanceToAddedSignal(Int_t label, Int_t nprim, 
//                                         Float_t photonE, Float_t photonEta, Float_t photonPhi)
//{
//  //Double_t addedY   = -100 ;
//  //Double_t addedE   = -1 ;
//  Double_t addedPt  = -1 ;
//  Double_t addedPhi =  100 ;
//  Double_t addedEta = -1 ;
//  
//  //Int_t    pdg       =  0 ;
//  Int_t    status    =  0 ;
//  Bool_t   inacceptance = kFALSE ;
//    
//  Int_t pdgMomOrg = 0, statusMomOrg = -1, momLabelOrg = -1;
//  Bool_t okMomOrg = kFALSE;
//  GetMCAnalysisUtils()->GetMother(label,GetMC(), pdgMomOrg, statusMomOrg, okMomOrg, momLabelOrg);     
//  
//  AliVParticle        * primary = 0;
//  AliVParticle        * primaryMom = 0;
//
//  TString genName;
//  (GetMC())->GetCocktailGenerator(label,genName);
//  
//  for(Int_t i=0 ; i < nprim; i++)
//  {
//    if( i == label ) continue ;
//    
//    if ( !GetReader()->AcceptParticleMCLabel( i ) ) continue ;
//    
//    //printf("particle %d\n",i);
//    
//    Int_t pdgMom = 0, statusMom = -1, momLabel = -1;
//    Bool_t okMom = kFALSE;
//    GetMCAnalysisUtils()->GetMother(i,GetMC(), pdgMom, statusMom, okMom, momLabel);    
//    //printf("\t mom label %d\n",momLabel);
//
//    if(momLabel < 0) continue;
//    
//    Int_t grandMomLabel = -1;
//      primary = GetMC()->Particle(i) ;
//      if(!primary)
//      {
//        AliWarning("primaries pointer not available!!");
//        continue;
//      }
//      
//      //pdg    = primary->GetPdgCode();
//      status = primary->GetStatusCode();
//      
//      // Protection against floating point exception
//      if ( primary->E() == TMath::Abs(primary->Pz()) || 
//          (primary->E() - primary->Pz()) < 1e-3      ||
//          (primary->E() + primary->Pz()) < 0           )  continue ; 
//      
//      //printf("i %d, %s %d  %s %d \n",i, GetMC()->Particle(i)->GetName(), GetMC()->Particle(i)->GetPdgCode(),
//      //       prim->GetName(), prim->GetPdgCode());
//      
//      // Photon kinematics
//      primary->Momentum(fMomentum);
//      
//      //addedY = 0.5*TMath::Log((primary->E+primary->Pz())/(primary->E-primary->Pz())) ;
//      
//      if(momLabel >= 0)
//      {
//        primaryMom = GetMC()->Particle(momLabel) ;
//        grandMomLabel = primaryMom->GetFirstMother();      
//      }
//    //printf("\t grandmom %d\n",grandMomLabel);
//   
//    // Select only photons in the final state
//    //if(pdg != 22 ) continue ;
//    
//    // If too small or too large pt, skip, same cut as for data analysis
//    addedPt  = fMomentum.Pt () ;
//    
//    if(addedPt < GetMinPt() || addedPt > GetMaxPt() ) continue ;
//    
//    //addedE   = fMomentum.E  () ;
//    addedEta = fMomentum.Eta() ;
//    addedPhi = fMomentum.Phi() ;
//    
//    if(addedPhi < 0) addedPhi+=TMath::TwoPi();
//    
//    // Check if photons hit desired acceptance
//    inacceptance = kTRUE;
//    
//    // Check same fidutial borders as in data analysis on top of real acceptance if real was requested.
//    if( IsFiducialCutOn() && !GetFiducialCut()->IsInFiducialCut(fMomentum.Eta(),fMomentum.Phi(),GetCalorimeter())) inacceptance = kFALSE ;
//    
//    // Check if photons hit the Calorimeter acceptance
//    if(IsRealCaloAcceptanceOn()) // defined on base class
//    {
//      if(!GetCaloUtils()->IsMCParticleInCalorimeterAcceptance(GetCalorimeter(), primary)) inacceptance = kFALSE ;
//    }
//    //printf("\t in acceptance %d", inacceptance);
//    if(!inacceptance) continue;
//    
//    TString genName2;
//    (GetMC())->GetCocktailGenerator(i,genName2);
//    
//    Float_t dEta = photonEta-addedEta;
//    Float_t dPhi = photonPhi-addedPhi;
//    Float_t distance = TMath::Sqrt(dEta*dEta + dPhi*dPhi);
//    
//    if (( momLabel != momLabelOrg && momLabelOrg >= 0) || momLabelOrg < 0)
//    {
//      if(!genName2.Contains("ijing"))
//      {
//        if(grandMomLabel < 0)
//        {
//          if ( !genName.Contains("ijing") ) fhDistanceAddedPhotonAddedPrimarySignal  ->Fill(photonE,distance,GetEventWeight());
//          if (  genName.Contains("ijing") ) fhDistanceHijingPhotonAddedPrimarySignal ->Fill(photonE,distance,GetEventWeight());
//        }
//        else
//        {
//          if ( !genName.Contains("ijing") ) fhDistanceAddedPhotonAddedSecondarySignal ->Fill(photonE,distance,GetEventWeight());
//          if (  genName.Contains("ijing") ) fhDistanceHijingPhotonAddedSecondarySignal->Fill(photonE,distance,GetEventWeight());
//        }
//      }
//    }
//    
//    if(!genName.Contains("ijing") &&  genName2.Contains("ijing") && status == 0) fhDistanceAddedPhotonHijingSecondary->Fill(photonE,distance,GetEventWeight());
//  }
//}
//
//________________________________________________________________________________
/// Fill some histograms to understand effect of pile-up.
//________________________________________________________________________________
void AliAnaPhoton::FillPileUpHistograms(AliVCluster* cluster, AliVCaloCells *cells,
                                        Int_t absIdMax)
{
  Float_t pt   = fMomentum.Pt();
  Float_t time = cluster->GetTOF()*1.e9;
  if ( time > 400 ) time-=fConstantTimeShift; // Only for MC, rest shift = 0
  
  AliVEvent * event = GetReader()->GetInputEvent();
  
  if(GetReader()->IsPileUpFromSPD())               fhPtPhotonPileUp[0]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromEMCal())             fhPtPhotonPileUp[1]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromSPDOrEMCal())        fhPtPhotonPileUp[2]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromSPDAndEMCal())       fhPtPhotonPileUp[3]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromSPDAndNotEMCal())    fhPtPhotonPileUp[4]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromEMCalAndNotSPD())    fhPtPhotonPileUp[5]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromNotSPDAndNotEMCal()) fhPtPhotonPileUp[6]->Fill(pt, GetEventWeight()) ;
  
  fhTimePtPhotonNoCut->Fill(pt, time, GetEventWeight());
  if(GetReader()->IsPileUpFromSPD()) fhTimePtPhotonSPD->Fill(pt, time, GetEventWeight());
  
  //
  // Cells inside the cluster
  //
    
  // Loop on cells inside cluster, max cell must be over 100 MeV and time in BC=0
  if(cells->GetCellAmplitude(absIdMax) > 0.1 && TMath::Abs(time) < 30)
  {
    for (Int_t ipos = 0; ipos < cluster->GetNCells(); ipos++)
    {
      Int_t absId  = cluster->GetCellsAbsId()[ipos];
      
      if( absId == absIdMax ) continue ;
      
      Double_t tcell = cells->GetCellTime(absId);
      Float_t  amp   = cells->GetCellAmplitude(absId);
      Int_t    bc    = GetReader()->GetInputEvent()->GetBunchCrossNumber();
      
      GetCaloUtils()->GetEMCALRecoUtils()->AcceptCalibrateCell(absId,bc,amp,tcell,cells);
      tcell*=1e9;
      if(tcell > 400)tcell-=fConstantTimeShift;
      
      Float_t diff = (time-tcell);
      
      if( cells->GetCellAmplitude(absIdMax) < 0.1 ) continue ;
      
      if(GetReader()->IsPileUpFromSPD())               fhClusterTimeDiffPhotonPileUp[0]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromEMCal())             fhClusterTimeDiffPhotonPileUp[1]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromSPDOrEMCal())        fhClusterTimeDiffPhotonPileUp[2]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromSPDAndEMCal())       fhClusterTimeDiffPhotonPileUp[3]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromSPDAndNotEMCal())    fhClusterTimeDiffPhotonPileUp[4]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromEMCalAndNotSPD())    fhClusterTimeDiffPhotonPileUp[5]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromNotSPDAndNotEMCal()) fhClusterTimeDiffPhotonPileUp[6]->Fill(pt, diff, GetEventWeight());

    } // loop
  }
  
  AliESDEvent* esdEv = dynamic_cast<AliESDEvent*> (event);
  AliAODEvent* aodEv = dynamic_cast<AliAODEvent*> (event);
  
  //
  // N pile up vertices
  //
  
  Int_t nVtxSPD = -1;
  Int_t nVtxTrk = -1;
  
  if      (esdEv)
  {
    nVtxSPD = esdEv->GetNumberOfPileupVerticesSPD();
    nVtxTrk = esdEv->GetNumberOfPileupVerticesTracks();
    
  } // ESD
  else if (aodEv)
  {
    nVtxSPD = aodEv->GetNumberOfPileupVerticesSPD();
    nVtxTrk = aodEv->GetNumberOfPileupVerticesTracks();
  } // AOD
  
  if(pt < 8)
  {
    fhTimeNPileUpVertSPD  ->Fill(time, nVtxSPD, GetEventWeight());
    fhTimeNPileUpVertTrack->Fill(time, nVtxTrk, GetEventWeight());
  }
  
  fhPtPhotonNPileUpSPDVtx->Fill(pt, nVtxSPD, GetEventWeight());
  fhPtPhotonNPileUpTrkVtx->Fill(pt, nVtxTrk, GetEventWeight());
	
  if(TMath::Abs(time) < 25)
  {
    fhPtPhotonNPileUpSPDVtxTimeCut->Fill(pt, nVtxSPD, GetEventWeight());
    fhPtPhotonNPileUpTrkVtxTimeCut->Fill(pt, nVtxTrk, GetEventWeight());
  }
	
  if(time < 75 && time > -25)
  {
    fhPtPhotonNPileUpSPDVtxTimeCut2->Fill(pt, nVtxSPD, GetEventWeight());
    fhPtPhotonNPileUpTrkVtxTimeCut2->Fill(pt, nVtxTrk, GetEventWeight());
  }
}

//_________________________________________________________________________________
/// Fill cluster Shower Shape histograms.
///
/// \param cluster : cluster pointer.
/// \param sm      : cluster super module number.
/// \param mctag   : tag containing MC origin of cluster.
/// \param weightPt: histogram weight depending on particle generated
/// \param nlm     : number of local maxima.
/// \param matched : is cluster matched to a track
/// \param maxCellFraction: max cell energy fraction in cluster
/// \param largeTime: number of cell with large time
//_________________________________________________________________________________
void  AliAnaPhoton::FillShowerShapeHistograms(AliVCluster* cluster, Int_t sm, 
                                              Int_t mcTag, Float_t weightPt,
                                              Int_t nlm, Bool_t matched,
                                              Float_t maxCellFraction, Int_t & largeTime)
{
  if ( !fFillSSHistograms || GetMixedEvent() ) return;
  
  Float_t energy  = cluster->E();
  Int_t   ncells  = cluster->GetNCells();
  Float_t lambda0 = cluster->GetM02();
  Float_t lambda1 = cluster->GetM20();
  Float_t disp    = cluster->GetDispersion()*cluster->GetDispersion();

  Float_t pt  = fMomentum.Pt();
  Float_t eta = fMomentum.Eta();
  Float_t phi = GetPhi(fMomentum.Phi());
  
  if ( !IsHighMultiplicityAnalysisOn() )
  {
    fhLam0E ->Fill(energy, lambda0, GetEventWeight()*weightPt);
    fhLam0Pt->Fill(pt    , lambda0, GetEventWeight()*weightPt);
    fhLam1E ->Fill(energy, lambda1, GetEventWeight()*weightPt);
    fhLam1Pt->Fill(pt    , lambda1, GetEventWeight()*weightPt);
  }
  else
  {
    fhLam0CentralityE->Fill(energy, lambda0, GetEventCentrality(), GetEventWeight()*weightPt);
  }
  
  if ( fFillSSPerSMHistograms )
  {
    fhLam0PerSM[sm]->Fill(pt, lambda0, GetEventWeight()*weightPt);
    fhLam1PerSM[sm]->Fill(pt, lambda1, GetEventWeight()*weightPt);
  }
  
  if ( !fFillOnlySimpleSSHisto )
  {
    fhDispE ->Fill(energy, disp   , GetEventWeight()*weightPt);
    fhDispPt->Fill(pt    , disp   , GetEventWeight()*weightPt);
  }
  
  if ( GetCalorimeter() == kEMCAL &&  GetFirstSMCoveredByTRD() >= 0 &&
     sm >= GetFirstSMCoveredByTRD()  )
  {
    fhLam0ETRD ->Fill(energy, lambda0, GetEventWeight()*weightPt);
    fhLam0PtTRD->Fill(pt    , lambda0, GetEventWeight()*weightPt);
    fhLam1ETRD ->Fill(energy, lambda1, GetEventWeight()*weightPt);
    if(!fFillOnlySimpleSSHisto && fFillSSHistograms)
      fhDispETRD ->Fill(energy, disp,    GetEventWeight()*weightPt);
  }
  
  //
  // EMCal SM regions
  //
  if ( cluster->IsEMCAL() && fFillEMCALRegionSSHistograms )
  {
//    Bool_t shared = GetCaloUtils()->IsClusterSharedByTwoSuperModules(GetEMCALGeometry(),cluster);
//    if(GetReader()->IsPileUpFromSPD())
//    {
//      fhLam0PerSMSPDPileUp[sm]->Fill(pt, lambda0, GetEventWeight()*weightPt);
//      fhLam1PerSMSPDPileUp[sm]->Fill(pt, lambda1, GetEventWeight()*weightPt);      
//    }

    Int_t etaRegion = -1, phiRegion = -1;
    GetCaloUtils()->GetEMCALSubregion(cluster,GetReader()->GetEMCALCells(),etaRegion,phiRegion);
    if(etaRegion >= 0 && etaRegion < 4 && phiRegion >=0 && phiRegion < 3) 
    {
      
      //      fhLam0EMCALRegion[etaRegion][phiRegion]->Fill(pt,lambda0, GetEventWeight()*weightPt);
      //      
      //      if(GetFirstSMCoveredByTRD() >= 0 && sm >= GetFirstSMCoveredByTRD()  )
      //      {
      //        fhLam0EMCALRegionTRD[etaRegion][phiRegion]->Fill(pt, lambda0, GetEventWeight()*weightPt);
      //      }
      
      fhLam0EMCALRegionPerSM[etaRegion][phiRegion][sm]->Fill(pt,lambda0, GetEventWeight()*weightPt);
      fhLam1EMCALRegionPerSM[etaRegion][phiRegion][sm]->Fill(pt,lambda1, GetEventWeight()*weightPt);
      
//      if(shared)
//      {
//        fhLam0PerSMShared[sm]->Fill(pt,lambda0, GetEventWeight()*weightPt);
//        fhLam1PerSMShared[sm]->Fill(pt,lambda1, GetEventWeight()*weightPt);
//      }
    }
    
    // Define l0 bin and cluster pt bin 
    Int_t l0bin = -1;
    if     ( lambda0 >=0.30 && lambda0 <= 0.40 ) l0bin = 1;
    else if( lambda0 >=0.23 && lambda0 <= 0.26 ) l0bin = 0;
    
    Float_t ptLimit[] = {2,3,4,5,6,8,10,12};
    Int_t ptbin = -1;
    for(Int_t ipt = 0; ipt < 7; ipt++)  
    {
      if( pt >= ptLimit[ipt] && pt < ptLimit[ipt+1]  )
      {
        ptbin = ipt;
        break;
      }
    }
    
    if(l0bin!=-1)
    {
      fhLam1Lam0BinPerSM[l0bin][sm]->Fill(pt,lambda1); 
      
      if(ptbin >= 0) 
      {
        fhEtaPhiLam0BinPtBin[l0bin][ptbin]->Fill(eta, phi, GetEventWeight()*weightPt);
//        if(shared) fhEtaPhiLam0BinPtBinSMShared[l0bin][ptbin]->Fill(eta, phi, GetEventWeight()*weightPt);
      }
    }
    
    // 
    // Histograms with time of secondary cells, only if they contribute with a non null 
    // weight to the shower shape
    //
    AliVCaloCells* cells = GetReader()->GetEMCALCells();
    
    Int_t    bc          = GetReader()->GetInputEvent()->GetBunchCrossNumber();
    
    //
    // Sort the cluster cells in energy, find maximum energy cell
    //
    Int_t ncell = cluster->GetNCells();
    Int_t    * sortList = new Int_t   [ncell];
    Float_t  * enerList = new Float_t [ncell];
    Double_t * timeList = new Double_t[ncell];

    for(Int_t icell = 0; icell < ncell; icell++)
    {
      Int_t absId = cluster->GetCellAbsId(icell);
      Float_t  cellE    = cells->GetCellAmplitude(absId);
      Double_t cellTime = cells->GetCellTime(absId);

      GetCaloUtils()->GetEMCALRecoUtils()->AcceptCalibrateCell(absId,bc,cellE,cellTime,cells);
      cellTime*=1e9;
      if(cellTime > 400)cellTime-=fConstantTimeShift;
      
      enerList[icell] = cellE;
      timeList[icell] = cellTime;
    }

    TMath::Sort(ncell,enerList,sortList); 

    //
    // Location of max cell
    // First in sorted list.
    //
    Int_t    absIdMax    = cluster->GetCellAbsId(sortList[0]);
    //    Float_t  cellEMax    = enerList[sortList[0]];
    Double_t cellTimeMax = timeList[sortList[0]];    
    
    //
    // Cluster cell loop, select only secondary cells with enough contribution to cluster
    // Start from second highest energy cell
    //
    largeTime  = 0;
    Float_t largeTimeE = 0;
    for(Int_t icell = 1; icell < ncell; icell++)
    {
      Int_t    absId    = cluster->GetCellAbsId(sortList[icell]);
      Float_t  cellE    = enerList[sortList[icell]];
      Double_t cellTime = timeList[sortList[icell]];

      //printf("\t icell %d, absId %d, ener %2.2f, time %2.2f, sort %d\n",icell,absId,cellE,cellTime,sortList[icell]);

      if ( absId == absIdMax ) continue;
      
      Float_t weight = GetCaloUtils()->GetEMCALRecoUtils()->GetCellWeight(cellE,cluster->E());
      
      //printf("Cluster E %2.2f, cell E %2.2f, time %2.2f, weight %2.3f\n",cluster->E(),cellE,cellTime,weight);
      if(weight < 0.01) continue;
      
      if ( TMath::Abs(cellTime) > 50 ) 
      {
        largeTime++;
        largeTimeE += cellE;
        //printf(" cluster pT %2.2f, cell E %2.2f, large time %d; ",pt, cellE, largeTime);
      }
      if ( l0bin == -1 ) continue;
          
      fhTimeLam0BinPerSM        [l0bin][sm]->Fill(pt, cellTime,        GetEventWeight()*weightPt);
      fhTimeLam0BinPerSMWeighted[l0bin][sm]->Fill(pt, cellTime, weight*GetEventWeight()*weightPt);
      
      fhDTimeLam0BinPerSM        [l0bin][sm]->Fill(pt, cellTimeMax-cellTime,        GetEventWeight()*weightPt);
      fhDTimeLam0BinPerSMWeighted[l0bin][sm]->Fill(pt, cellTimeMax-cellTime, weight*GetEventWeight()*weightPt);
      
      fhCellClusterEFracLam0BinPerSM        [l0bin][sm]->Fill(pt, cellE/cluster->E(),        GetEventWeight()*weightPt);
//      fhCellClusterEFracLam0BinPerSMWeighted[l0bin][sm]->Fill(pt, cellE/cluster->E(), weight*GetEventWeight()*weightPt);        
      
      fhCellClusterELam0BinPerSM        [l0bin][sm]->Fill(pt, cellE,        GetEventWeight()*weightPt);
      fhCellClusterELam0BinPerSMWeighted[l0bin][sm]->Fill(pt, cellE, weight*GetEventWeight()*weightPt);
      
      if(ptbin >= 0) 
      {
        fhCellClusterIndexEAndTime[l0bin][ptbin] ->Fill(cellTime, icell             , GetEventWeight()*weightPt);
        fhCellClusterEAndTime     [l0bin][ptbin] ->Fill(cellTime, cellE             , GetEventWeight()*weightPt);
        fhCellClusterEFracAndTime [l0bin][ptbin] ->Fill(cellTime, cellE/cluster->E(), GetEventWeight()*weightPt);
      }
      
      if ( TMath::Abs(cellTime) > 50 )
      {        
        fhCellClusterEFracLam0BinPerSMLargeTime [l0bin][sm]->Fill(pt, cellE/cluster->E(), GetEventWeight()*weightPt);
        fhCellClusterELam0BinPerSMLargeTime     [l0bin][sm]->Fill(pt, cellE             , GetEventWeight()*weightPt);
        fhCellClusterIndexELam0BinPerSMLargeTime[l0bin][sm]->Fill(pt, icell             , GetEventWeight()*weightPt);        
      }
//      if(shared)
//        fhTimeLam0BinPerSMShared[l0bin][sm]->Fill(pt, cellTime, GetEventWeight()*weightPt);
      
      if(ptbin >= 0) 
      {
        Int_t   icol     = -1, icolAbs = -1;
        Int_t   irow     = -1, irowAbs = -1;
        Int_t   iRCU     = -1;
        GetModuleNumberCellIndexesAbsCaloMap(absId,GetCalorimeter(), icol, irow, iRCU, icolAbs, irowAbs);
        
        fhColRowLam0BinPtBin        [l0bin][ptbin]->Fill(icolAbs, irowAbs,        GetEventWeight()*weightPt);
        fhColRowLam0BinPtBinWeighted[l0bin][ptbin]->Fill(icolAbs, irowAbs, weight*GetEventWeight()*weightPt);
        
        if ( TMath::Abs(cellTime) > 50 )
          fhColRowLam0BinPtBinLargeTime[l0bin][ptbin]->Fill(icolAbs, irowAbs, GetEventWeight()*weightPt);
        
//        if(shared)  fhColRowLam0BinPtBinSMShared[l0bin][ptbin]->Fill(icolAbs, irowAbs, GetEventWeight()*weightPt);
      }
      
      
    } // cluster cell loop
      //if(largeTime > 0) printf("\n");
    
    if ( largeTime > 0 ) 
    {
      if ( l0bin != -1 ) 
      {
        if ( ptbin > 0 ) 
          fhEtaPhiLargeTimeInClusterCell[l0bin][ptbin]->Fill(eta, phi, GetEventWeight()*weightPt);
        
        fhCellClusterEFracLam0BinPerSMLargeTimeTotal[l0bin][sm]->Fill(pt, largeTimeE/cluster->E(), GetEventWeight()*weightPt);
        fhNCellsWithLargeTimeInCluster              [l0bin][sm]->Fill(pt, largeTime              , GetEventWeight()*weightPt);
      }
      
      fhLam0PerSMLargeTimeInClusterCell[sm]->Fill(pt, lambda0, GetEventWeight()*weightPt);
      fhLam1PerSMLargeTimeInClusterCell[sm]->Fill(pt, lambda1, GetEventWeight()*weightPt);
      
      if(largeTime < 6)
      {
        fhLam0PerNLargeTimeInClusterCell[largeTime-1]->Fill(pt, lambda0, GetEventWeight()*weightPt);
        fhLam1PerNLargeTimeInClusterCell[largeTime-1]->Fill(pt, lambda1, GetEventWeight()*weightPt);
      }
    }
    
    delete [] sortList ;
    delete [] enerList ;
    delete [] timeList ;
    //printf("Cluster %d, E %2.2f, sm %d, eta %2.2f, phi %2.2f ---> region %d %d\n",cluster->GetID(),cluster->E(),sm,eta,RadToDeg(phi),etaRegion,phiRegion);
  } // Check EMCal SM and regions
  
  Float_t l0   = 0., l1   = 0.;
  Float_t dispp= 0., dEta = 0., dPhi    = 0.;
  Float_t sEta = 0., sPhi = 0., sEtaPhi = 0.;
  if ( GetCalorimeter() == kEMCAL && !fFillOnlySimpleSSHisto )
  {
    GetCaloUtils()->GetEMCALRecoUtils()->RecalculateClusterShowerShapeParameters(GetEMCALGeometry(), GetReader()->GetInputEvent()->GetEMCALCells(), cluster,
                                                                                 l0, l1, dispp, dEta, dPhi, sEta, sPhi, sEtaPhi);
    //printf("AliAnaPhoton::FillShowerShapeHistogram - l0 %2.6f, l1 %2.6f, disp %2.6f, dEta %2.6f, dPhi %2.6f, sEta %2.6f, sPhi %2.6f, sEtaPhi %2.6f \n",
    //       l0, l1, dispp, dEta, dPhi, sEta, sPhi, sEtaPhi );
    //printf("AliAnaPhoton::FillShowerShapeHistogram - dispersion %f, dispersion eta+phi %f \n",
    //       disp, dPhi+dEta );
    fhDispEtaE        -> Fill(energy, dEta     , GetEventWeight()*weightPt);
    fhDispPhiE        -> Fill(energy, dPhi     , GetEventWeight()*weightPt);
    fhSumEtaE         -> Fill(energy, sEta     , GetEventWeight()*weightPt);
    fhSumPhiE         -> Fill(energy, sPhi     , GetEventWeight()*weightPt);
    fhSumEtaPhiE      -> Fill(energy, sEtaPhi  , GetEventWeight()*weightPt);
    fhDispEtaPhiDiffE -> Fill(energy, dPhi-dEta, GetEventWeight()*weightPt);
      
    if(dEta+dPhi>0)fhSphericityE     -> Fill(energy,(dPhi-dEta)/(dEta+dPhi)     , GetEventWeight()*weightPt);
    if(dEta+sEta>0)fhDispSumEtaDiffE -> Fill(energy,(dEta-sEta)/((dEta+sEta)/2.), GetEventWeight()*weightPt);
    if(dPhi+sPhi>0)fhDispSumPhiDiffE -> Fill(energy,(dPhi-sPhi)/((dPhi+sPhi)/2.), GetEventWeight()*weightPt);
    
    Int_t ebin = -1;
    if      (energy < 2 ) ebin = 0;
    else if (energy < 4 ) ebin = 1;
    else if (energy < 6 ) ebin = 2;
    else if (energy < 10) ebin = 3;
    else if (energy < 15) ebin = 4;
    else if (energy < 20) ebin = 5;
    else                  ebin = 6;
    
    fhDispEtaDispPhi[ebin]->Fill(dEta   , dPhi, GetEventWeight()*weightPt);
    fhLambda0DispEta[ebin]->Fill(lambda0, dEta, GetEventWeight()*weightPt);
    fhLambda0DispPhi[ebin]->Fill(lambda0, dPhi, GetEventWeight()*weightPt);
  }
  
  // If track-matching was off, check effect of track-matching cut
  //
  if ( !fRejectTrackMatch && matched )
  {
    fhLam0ETM ->Fill(energy, lambda0, GetEventWeight()*weightPt);
    fhLam0PtTM->Fill(pt    , lambda0, GetEventWeight()*weightPt);
    fhLam1ETM ->Fill(energy, lambda1, GetEventWeight()*weightPt);
    if(!fFillOnlySimpleSSHisto && fFillSSHistograms)
      fhDispETM ->Fill(energy, disp   , GetEventWeight()*weightPt);
    
    if(GetCalorimeter() == kEMCAL &&  GetFirstSMCoveredByTRD() >= 0 &&
       sm >= GetFirstSMCoveredByTRD()  )
    {
      fhLam0ETMTRD ->Fill(energy, lambda0, GetEventWeight()*weightPt);
      fhLam0PtTMTRD->Fill(pt    , lambda0, GetEventWeight()*weightPt);
      fhLam1ETMTRD ->Fill(energy, lambda1, GetEventWeight()*weightPt);
      if(!fFillOnlySimpleSSHisto && fFillSSHistograms)
        fhDispETMTRD ->Fill(energy, disp   , GetEventWeight()*weightPt);
    }
    
  } // If track-matching was off, check effect of matching residual cut
  
  if(!fFillOnlySimpleSSHisto)
  {
    if(energy < 2)
    {
      fhNCellsLam0LowE ->Fill(ncells, lambda0, GetEventWeight()*weightPt);
      fhNCellsLam1LowE ->Fill(ncells, lambda1, GetEventWeight()*weightPt);
      fhNCellsDispLowE ->Fill(ncells, disp   , GetEventWeight()*weightPt);
      
      fhLam1Lam0LowE  ->Fill(lambda1, lambda0, GetEventWeight()*weightPt);
      fhLam0DispLowE  ->Fill(lambda0, disp   , GetEventWeight()*weightPt);
      fhDispLam1LowE  ->Fill(disp   , lambda1, GetEventWeight()*weightPt);
      fhEtaLam0LowE   ->Fill(eta    , lambda0, GetEventWeight()*weightPt);
      fhPhiLam0LowE   ->Fill(phi    , lambda0, GetEventWeight()*weightPt);
    }
    else
    {
      fhNCellsLam0HighE ->Fill(ncells, lambda0, GetEventWeight()*weightPt);
      fhNCellsLam1HighE ->Fill(ncells, lambda1, GetEventWeight()*weightPt);
      fhNCellsDispHighE ->Fill(ncells, disp   , GetEventWeight()*weightPt);
      
      fhLam1Lam0HighE  ->Fill(lambda1, lambda0, GetEventWeight()*weightPt);
      fhLam0DispHighE  ->Fill(lambda0, disp   , GetEventWeight()*weightPt);
      fhDispLam1HighE  ->Fill(disp   , lambda1, GetEventWeight()*weightPt);
      fhEtaLam0HighE   ->Fill(eta    , lambda0, GetEventWeight()*weightPt);
      fhPhiLam0HighE   ->Fill(phi    , lambda0, GetEventWeight()*weightPt);
    }
  }
  
  if ( IsDataMC() )
  {
    AliVCaloCells* cells = 0;
    if(GetCalorimeter() == kEMCAL) cells = GetEMCALCells();
    else                           cells = GetPHOSCells();
    
    // Fill histograms to check shape of embedded clusters
    Float_t fraction = 0;
    // printf("check embedding %i\n",GetReader()->IsEmbeddedClusterSelectionOn());
    
    if ( IsEmbedingAnalysisOn() )      
    {
      // Only working for EMCAL
      // 	printf("embedded\n");
        
      Float_t clusterE = 0; // recalculate in case corrections applied.
      Float_t cellE    = 0;
//      printf("N cells combined %d, data %d, MC %d, data+MC-combined %d\n",
//                    cells->GetNumberOfCells(),
//                    GetReader()->GetInputEvent()->GetEMCALCells()->GetNumberOfCells(),
//                    GetReader()->GetEMCALCellsExternalEvent()->GetNumberOfCells(),
//             GetReader()->GetInputEvent()->GetEMCALCells()->GetNumberOfCells()+GetReader()->GetEMCALCellsExternalEvent()->GetNumberOfCells()-cells->GetNumberOfCells());
      if ( !GetReader()->IsEmbeddedMCEventUsed() )
      {
        for(Int_t icell  = 0; icell < cluster->GetNCells(); icell++)
        {
          cellE    = cells->GetCellAmplitude(cluster->GetCellAbsId(icell));
          clusterE+=cellE;
          fraction+=cellE*cluster->GetCellAmplitudeFraction(icell);
        }
        // Fraction of total energy due to the embedded signal
        fraction/=clusterE;
      }
      else if ( !GetReader()->IsEmbeddedInputEventUsed() )
      {
        Float_t sigCellE    = 0; 
        Float_t sigClusterE = 0; 

//        Float_t comCellE    = 0;
//        Float_t extCellE    = 0;
//        Float_t datCellE    = 0;
//        Float_t comClusterE = 0; 
//        Float_t extClusterE = 0; 
//        Float_t datClusterE = 0; 
        for(Int_t icell  = 0; icell < cluster->GetNCells(); icell++)
        {
          Int_t id = cluster->GetCellAbsId(icell);
          //extCellE = GetReader()->GetEMCALCellsExternalEvent()->GetCellAmplitude(id); // cells from external event
          //datCellE = GetReader()->GetInputEvent()->GetEMCALCells()->GetCellAmplitude(id);  // cells from data
          
          cellE    = cells->GetCellAmplitude(id);//extCellE+datCellE;
          sigCellE = cells->GetCellEFraction(id); // MC signal

//          Short_t cellNumber = -1;
//          Double_t amplitude = 0;
//          Double_t time = 0;
//          Double_t eFrac = 0;
//          Int_t mcLabel = 0;
//          comCellE = 0;
//          for(Int_t icell  = 0; icell < cells->GetNumberOfCells(); icell++)
//          {
//            cells->GetCell(icell, cellNumber, amplitude, time, mcLabel, eFrac);
//            if ( id == cellNumber ) comCellE += amplitude;
//          }
          
          clusterE   += cellE   ;
//          extClusterE+= extCellE;
//          datClusterE+= datCellE;
//          comClusterE+= comCellE;
          sigClusterE+= sigCellE;
        }
        
        // Fraction of total energy due to the embedded signal
        //fraction = extClusterE / clusterE;
        fraction = sigClusterE / clusterE;
   
//        printf("Embedded signal E fraction %2.3f, Energy: Reco %2.3f,  sum cell %2.3f, combi %2.3f, ext+data %2.3f,"
//                        " ext %2.3f, sig %2.3f, data %2.3f - "
//                        "Fraction sum/reco %2.3f - Fraction sum/combi %2.3f, ext/signal %2.3f \n",
//                        fraction, energy, clusterE, comClusterE, datClusterE+extClusterE,
//                        extClusterE, sigClusterE, datClusterE,  
//                        clusterE/energy, clusterE/comClusterE, sigClusterE/extClusterE);
        
        if ( fraction > 1 )
        {
          printf("Careful! Embedded signal E fraction %2.3f,  sum cell %2.3f, sum sig %2.3f \n",
                 fraction, clusterE, sigClusterE);
        }
      }
      
      AliDebug(1,Form("Energy fraction of embedded signal %2.3f, Energy %2.3f", fraction, clusterE));
      
      fhEmbeddedSignalFractionEnergy->Fill(energy, fraction, GetEventWeight()*weightPt);
      if ( IsHighMultiplicityAnalysisOn() )
        fhEmbeddedSignalFractionEnergyPerCentrality->Fill(energy, fraction, GetEventCentrality(), GetEventWeight()*weightPt);

    }  // embedded fraction
      
    //
    // Check the origin 
    //
    Int_t mcIndex = -1;
    Bool_t conversion = GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCConversion);
    if ( !fSeparateConvertedDistributions ) conversion = kFALSE;
    
    if( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCPhoton)     &&
       !GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCPi0)        &&
       !GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCEta))
    {
      if   ( !conversion ) mcIndex = kmcssPhoton    ;
      else                 mcIndex = kmcssPhotonConv;
      
      // Fill histograms to check shape of embedded clusters
      if ( IsEmbedingAnalysisOn() )
      {
        fhEmbeddedPhotonFractionEnergy   ->Fill(energy, fraction,          GetEventWeight()*weightPt);
        fhEmbeddedPhotonFractionEnergyM02->Fill(energy, fraction, lambda0, GetEventWeight()*weightPt);
      } // embedded
      
      if ( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCPrompt) )
      {
        if   ( !conversion ) mcIndex = kmcssPhotonPrompt    ;
        else                 mcIndex = kmcssPhotonPromptConv;
      } // prompt photon
      else if ( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCFragmentation) )
      {
        if   ( !conversion ) mcIndex = kmcssPhotonFrag    ;
        else                 mcIndex = kmcssPhotonFragConv;
      } // fragmentation photon
    } // photon 
    else if  ( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCElectron))
    {
      mcIndex = kmcssElectron ;
    } // electron
    else if  ( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCPi0)  )
    {
      if   ( !conversion ) mcIndex = kmcssPi0    ;
      else                 mcIndex = kmcssPi0Conv;

      // Fill histograms to check shape of embedded clusters
      if ( IsEmbedingAnalysisOn() )
      {
        fhEmbeddedPi0FractionEnergy   ->Fill(energy, fraction,          GetEventWeight()*weightPt);
        fhEmbeddedPi0FractionEnergyM02->Fill(energy, fraction, lambda0, GetEventWeight()*weightPt);
      } // embedded
      
    } // pi0
    else if  ( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCEta)  )
    {
      if   ( !conversion ) mcIndex = kmcssEta    ;
      else                 mcIndex = kmcssEtaConv;
      
    }//eta
    else
    {
      mcIndex = kmcssOther ;
    } // other particles
    
    //
    // Fill histograms
    //
    fhMCPtLambda0[mcIndex]->Fill(pt    , lambda0, GetEventWeight()*weightPt);
    
    if (!fFillOnlyPtHisto )
    {
      fhMCELambda0 [mcIndex]->Fill(energy, lambda0, GetEventWeight()*weightPt);
      fhMCELambda1 [mcIndex]->Fill(energy, lambda1, GetEventWeight()*weightPt);
    }
    
    if ( !fFillOnlySimpleSSHisto ) 
    {
      fhMCNCellsE            [mcIndex]->Fill(energy, ncells , GetEventWeight()*weightPt);
      fhMCMaxCellDiffClusterE[mcIndex]->Fill(energy, maxCellFraction, GetEventWeight()*weightPt);
      fhMCEDispersion        [mcIndex]->Fill(energy, disp   , GetEventWeight()*weightPt);
    }
    
    //
    // Check particle overlaps in cluster
    //
    if ( !IsEmbedingAnalysisOn() && fCheckOverlaps )
    {    
      // Compare the primary depositing more energy with the rest,
      // if no photon/electron as comon ancestor (conversions), count as other particle
      const UInt_t nlabels = cluster->GetNLabels();
      Int_t overpdg[nlabels];
      Int_t overlab[nlabels];
      Int_t noverlaps = GetMCAnalysisUtils()->GetNOverlaps(cluster->GetLabels(), nlabels,mcTag,-1,GetMC(),overpdg,overlab);
      
      //printf("N overlaps %d \n",noverlaps);

      if(noverlaps == 0)
        fhMCPtLambda0Overlaps[mcIndex][0]->Fill(pt, lambda0, GetEventWeight()*weightPt);
      else if(noverlaps == 1)
        fhMCPtLambda0Overlaps[mcIndex][1]->Fill(pt, lambda0, GetEventWeight()*weightPt);
      else if(noverlaps > 1)
        fhMCPtLambda0Overlaps[mcIndex][2]->Fill(pt, lambda0, GetEventWeight()*weightPt);
      else
        AliWarning(Form("n overlaps = %d!!", noverlaps));
    }
    
    if ( !fFillOnlySimpleSSHisto )
    {
      if     (energy < 2.)
      {
        fhMCLambda0vsClusterMaxCellDiffE0[mcIndex]->Fill(lambda0, maxCellFraction, GetEventWeight()*weightPt);
        fhMCNCellsvsClusterMaxCellDiffE0 [mcIndex]->Fill(ncells,  maxCellFraction, GetEventWeight()*weightPt);
      }
      else if(energy < 6.)
      {
        fhMCLambda0vsClusterMaxCellDiffE2[mcIndex]->Fill(lambda0, maxCellFraction, GetEventWeight()*weightPt);
        fhMCNCellsvsClusterMaxCellDiffE2 [mcIndex]->Fill(ncells,  maxCellFraction, GetEventWeight()*weightPt);
      }
      else
      {
        fhMCLambda0vsClusterMaxCellDiffE6[mcIndex]->Fill(lambda0, maxCellFraction, GetEventWeight()*weightPt);
        fhMCNCellsvsClusterMaxCellDiffE6 [mcIndex]->Fill(ncells,  maxCellFraction, GetEventWeight()*weightPt);
      }
      
      if(GetCalorimeter() == kEMCAL)
      {
        fhMCEDispEta        [mcIndex]-> Fill(energy, dEta     , GetEventWeight()*weightPt);
        fhMCEDispPhi        [mcIndex]-> Fill(energy, dPhi     , GetEventWeight()*weightPt);
        fhMCESumEtaPhi      [mcIndex]-> Fill(energy, sEtaPhi  , GetEventWeight()*weightPt);
        fhMCEDispEtaPhiDiff [mcIndex]-> Fill(energy, dPhi-dEta, GetEventWeight()*weightPt);
          
        if( dEta+dPhi > 0 ) fhMCESphericity[mcIndex]-> Fill(energy, (dPhi-dEta)/(dEta+dPhi), GetEventWeight()*weightPt);
        
        Int_t ebin = -1;
        if      (energy < 2 ) ebin = 0;
        else if (energy < 4 ) ebin = 1;
        else if (energy < 6 ) ebin = 2;
        else if (energy < 10) ebin = 3;
        else if (energy < 15) ebin = 4;
        else if (energy < 20) ebin = 5;
        else                  ebin = 6;
        
        fhMCDispEtaDispPhi[ebin][mcIndex]->Fill(dEta   , dPhi, GetEventWeight()*weightPt);
        fhMCLambda0DispEta[ebin][mcIndex]->Fill(lambda0, dEta, GetEventWeight()*weightPt);
        fhMCLambda0DispPhi[ebin][mcIndex]->Fill(lambda0, dPhi, GetEventWeight()*weightPt);
      }
    }
  }// MC data
}

//__________________________________________________________________________
/// If selected, fill histograms with residuals of matched clusters,
/// help to define track matching cut.
/// Residual filled for different cuts:
/// \param cluster : pointer to cluster info.
/// \param cut     : 0, No cut, 1, after cluster selection cuts.
/// \param nSMod   : cluster sm number.
/// \param matched : is cluster matched (useful in cut case 0)
/// \param bEoP: If rejection is due to E over P cut, set it true, else false
/// \param bRes: If rejection is due to residual eta-phi cut, set it true, else false
/// \param tag     : tag containing MC origin of cluster.
/// \param mcbin   : particle tag for MC histogram
/// \param egen    : main MC particle generated energy
/// \param noverlaps: number of extra MC particles contributing 
/// \param weightPt: histogram weight depending on particle generated
//__________________________________________________________________________
void AliAnaPhoton::FillTrackMatchingResidualHistograms(AliVCluster* cluster,
                                                       Int_t cut, Int_t nSMod, Bool_t matched, 
                                                       Bool_t bEoP, Bool_t bRes,
                                                       Int_t tag, Float_t mcbin, Float_t egen,
                                                       Int_t noverlaps, Float_t weightPt)
{
  Float_t dEta = cluster->GetTrackDz();
  Float_t dPhi = cluster->GetTrackDx();
  Float_t ener = cluster->E();
//  if(cluster->IsEMCAL() && GetCaloUtils()->IsRecalculationOfClusterTrackMatchingOn())
//  {
//    dPhi = 2000., dEta = 2000.;
//    GetCaloUtils()->GetEMCALRecoUtils()->GetMatchedPhiesiduals(cluster->GetID(),dEta,dPhi);
//  }
  
  AliVTrack *track = GetCaloUtils()->GetMatchedTrack(cluster, GetReader()->GetInputEvent());

  if ( TMath::Abs(dPhi) > 999 || !track ) return;
  
  Bool_t positive = (track->Charge()>0);
  
  Int_t icent = GetEventCentralityBin();
  if ( !IsHighMultiplicityAnalysisOn() )
  {
    if ( positive )
    {
      fhTrackMatchedDEtaDPhiPos[cut]->Fill(ener,dEta,dPhi, GetEventWeight()*weightPt);
      
      if ( fFillTMHistoTrackPt )
        fhTrackMatchedDEtaDPhiPosTrackPt[cut]->Fill(track->Pt(),dEta,dPhi, GetEventWeight()*weightPt);
    }
    else
    {
      fhTrackMatchedDEtaDPhiNeg[cut]->Fill(ener, dEta, dPhi, GetEventWeight()*weightPt);
      
      if ( fFillTMHistoTrackPt )
        fhTrackMatchedDEtaDPhiNegTrackPt[cut]->Fill(track->Pt(), dEta, dPhi, GetEventWeight()*weightPt);
    }
  }
  else if ( positive  && icent >= 0 && GetNCentrBin() > 0  && icent < GetNCentrBin() )
  {
    fhTrackMatchedDEtaDPhiPosPerCen[icent]->Fill(ener,dEta,dPhi, GetEventWeight()*weightPt);

    if ( fFillTMHistoTrackPt )
      fhTrackMatchedDEtaDPhiPosTrackPtPerCen[icent]->Fill(track->Pt(),dEta,dPhi, GetEventWeight()*weightPt);
  }
    
  // TRD
  if( GetCalorimeter() == kEMCAL &&   GetFirstSMCoveredByTRD() >= 0 &&
     nSMod >= GetFirstSMCoveredByTRD()   )
  {
    fhTrackMatchedDEtaTRD[cut]->Fill(ener, dEta, GetEventWeight()*weightPt);
    fhTrackMatchedDPhiTRD[cut]->Fill(ener, dPhi, GetEventWeight()*weightPt);
  }
    
  // Check dEdx and E/p and MC origin of matched/unmatched clusters
  //
  if ( IsDataMC() )
  {
    fhTrackMatchedMCParticleBeforeTM       [cut]->Fill(ener       , mcbin, GetEventWeight()*weightPt);
    fhTrackMatchedMCParticleTrackPtBeforeTM[cut]->Fill(track->Pt(), mcbin, GetEventWeight()*weightPt);
  }
  
  //if ( (matched && !cut) || (!matched && cut) )
  {
    Float_t dEdx   = track->GetTPCsignal();
    Float_t eOverp = -1;
    if ( track->P() > 0.1 ) eOverp = ener/track->P();
    
    //        if ( cut )
    //          printf("matched %d and cut %d: E %2.2f, track p %2.2f, track pt %2.2f, dEta %2.2f, dPhi %2.2f, dEdx %2.2f, E/p %2.2f\n",
    //                 matched,cut,ener,track->P(),track->Pt(),dEta,dPhi,dEdx,eOverp);
    
    if ( !IsHighMultiplicityAnalysisOn() )
    {
      fhdEdx  [cut]->Fill(ener, dEdx  , GetEventWeight()*weightPt);
      fhEOverP[cut]->Fill(ener, eOverp, GetEventWeight()*weightPt);
      
      if ( cut == 0 )
      {
        if ( bRes ) 
        {
          fhEOverPAfterResidualCut->Fill(ener, eOverp, GetEventWeight()*weightPt);
          if ( fFillTMHistoTrackPt )
            fhEOverPTrackPtAfterResidualCut->Fill(track->Pt(), eOverp, GetEventWeight()*weightPt);
        }
        
        if ( bEoP && positive )
        {
          fhTrackMatchedDEtaDPhiPosAfterEOverPCut->Fill(ener, dEta, dPhi, GetEventWeight()*weightPt);
          if ( fFillTMHistoTrackPt )
            fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCut->Fill(track->Pt(), dEta, dPhi, GetEventWeight()*weightPt);
        }
      }
      
      if ( fFillTMHistoTrackPt )
      {
        fhdEdxTrackPt  [cut]->Fill(track->Pt(), dEdx  , GetEventWeight()*weightPt);
        fhEOverPTrackPt[cut]->Fill(track->Pt(), eOverp, GetEventWeight()*weightPt);
      }
    }
    else // centrality dependent
    {
      fhEOverPCentrality->Fill(ener, eOverp, GetEventWeight()*weightPt);
      if ( fFillTMHistoTrackPt )
        fhEOverPCentralityTrackPt->Fill(track->Pt(), eOverp, GetEventWeight()*weightPt);
      
      if ( bRes ) 
      {
        fhEOverPCentralityAfterResidualCut->Fill(ener, eOverp, GetEventWeight()*weightPt);
        if ( fFillTMHistoTrackPt )
          fhEOverPCentralityTrackPtAfterResidualCut->Fill(track->Pt(), eOverp, GetEventWeight()*weightPt);
      }

      if ( bEoP && positive && 
          icent >= 0 && GetNCentrBin() > 0 && icent < GetNCentrBin() )
      {
        fhTrackMatchedDEtaDPhiPosAfterEOverPCutPerCen[icent]->Fill(ener, dEta, dPhi, GetEventWeight()*weightPt);
        if ( fFillTMHistoTrackPt )
          fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCutPerCen[icent]->Fill(track->Pt(), dEta, dPhi, GetEventWeight()*weightPt);
      }
    } // centrality dependent
    
    if(GetCalorimeter()==kEMCAL &&  GetFirstSMCoveredByTRD() >= 0 &&
       nSMod >= GetFirstSMCoveredByTRD()  )
      fhEOverPTRD[cut]->Fill(ener, eOverp, GetEventWeight()*weightPt);
    
    if ( IsDataMC() )
    {
      fhTrackMatchedMCParticle        [cut]->Fill(ener, mcbin, GetEventWeight()*weightPt);
      fhTrackMatchedMCParticleTrackPt [cut]->Fill(track->Pt(), mcbin, GetEventWeight()*weightPt);
      fhTrackMatchedMCParticleVsEOverP[cut]->Fill(ener, mcbin, eOverp, GetEventWeight()*weightPt);
      
      if ( egen > 0.1 )
        fhTrackMatchedMCParticleVsErecEgen[cut]->Fill(ener, mcbin, ener/egen, GetEventWeight()*weightPt);
      
      Bool_t conversion = GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCConversion);
      if  ( conversion  )
        fhTrackMatchedMCParticleConverted[cut]->Fill(ener, mcbin, GetEventWeight()*weightPt);
     
      if ( fCheckOverlaps )
        fhTrackMatchedMCParticleVsNOverlaps[cut]->Fill(ener, mcbin, noverlaps, GetEventWeight()*weightPt);
      
      if(!GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0) &&
         !GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEta) && fCheckOverlaps )
      {
        if ( noverlaps )
        {
          fhTrackMatchedDEtaMCNoOverlap[cut]->Fill(ener, dEta, GetEventWeight()*weightPt);
          fhTrackMatchedDPhiMCNoOverlap[cut]->Fill(ener, dPhi, GetEventWeight()*weightPt);
        }
        else
        {
          fhTrackMatchedDEtaMCOverlap[cut]->Fill(ener, dEta, GetEventWeight()*weightPt);
          fhTrackMatchedDPhiMCOverlap[cut]->Fill(ener, dPhi, GetEventWeight()*weightPt);
        }
        
        if( conversion )
        {
          fhTrackMatchedDEtaMCConversion[cut]->Fill(ener, dEta, GetEventWeight()*weightPt);
          fhTrackMatchedDPhiMCConversion[cut]->Fill(ener, dPhi, GetEventWeight()*weightPt);
        }
        
      }// Check overlaps
      
    } // MC
    
  } // track info for clusters matched and not matched
    
}

//___________________________________________
/// Save parameters used for analysis in a string.
//___________________________________________
TObjString *  AliAnaPhoton::GetAnalysisCuts()
{
  TString parList ; //this will be list of parameters used for this analysis.
  const Int_t buffersize = 255;
  char onePar[buffersize] ;
  
  snprintf(onePar,buffersize,"--- AliAnaPhoton ---:") ;
  parList+=onePar ;
  snprintf(onePar,buffersize,"Calorimeter: %s;",GetCalorimeterString().Data()) ;
  parList+=onePar ;
  snprintf(onePar,buffersize,"fMinDist =%2.2f (Minimal distance to bad channel to accept cluster);",fMinDist) ;
  parList+=onePar ;
  snprintf(onePar,buffersize,"fMinDist2=%2.2f (Cuts on Minimal distance to study acceptance evaluation);",fMinDist2) ;
  parList+=onePar ;
  snprintf(onePar,buffersize,"fMinDist3=%2.2f (One more cut on distance used for acceptance-efficiency study);",fMinDist3) ;
  parList+=onePar ;
  snprintf(onePar,buffersize,"fRejectTrackMatch: %d",fRejectTrackMatch) ;
  parList+=onePar ;
  
  // Get parameters set in base class.
  parList += GetBaseParametersList() ;
  
  // Get parameters set in PID class.
  parList += GetCaloPID()->GetPIDParametersList() ;
  
  // Get parameters set in FiducialCut class (not available yet)
  //parlist += GetFidCut()->GetFidCutParametersList()
  
  return new TObjString(parList) ;
}

//_____________________________________________
/// Create histograms to be saved in output file and
/// store them in outputContainer.
//_____________________________________________
TList *  AliAnaPhoton::GetCreateOutputObjects()
{
  TList * outputContainer = new TList() ;
  outputContainer->SetName("PhotonHistos") ;
	  
  Int_t nptbins  = GetHistogramRanges()->GetHistoPtBins();  Float_t ptmax  = GetHistogramRanges()->GetHistoPtMax();  Float_t ptmin  = GetHistogramRanges()->GetHistoPtMin();
  Int_t nphibins = GetHistogramRanges()->GetHistoPhiBins(); Float_t phimax = GetHistogramRanges()->GetHistoPhiMax(); Float_t phimin = GetHistogramRanges()->GetHistoPhiMin();
  Int_t netabins = GetHistogramRanges()->GetHistoEtaBins(); Float_t etamax = GetHistogramRanges()->GetHistoEtaMax(); Float_t etamin = GetHistogramRanges()->GetHistoEtaMin();
  Int_t ssbins   = GetHistogramRanges()->GetHistoShowerShapeBins();  Float_t ssmax   = GetHistogramRanges()->GetHistoShowerShapeMax();  Float_t ssmin   = GetHistogramRanges()->GetHistoShowerShapeMin();
  Int_t nbins    = GetHistogramRanges()->GetHistoNClusterCellBins(); Int_t   nmax    = GetHistogramRanges()->GetHistoNClusterCellMax(); Int_t   nmin    = GetHistogramRanges()->GetHistoNClusterCellMin();
  Int_t ntimebins= GetHistogramRanges()->GetHistoTimeBins();         Float_t timemax = GetHistogramRanges()->GetHistoTimeMax();         Float_t timemin = GetHistogramRanges()->GetHistoTimeMin();
  
  Int_t   nresetabins = GetHistogramRanges()->GetHistoTrackResidualEtaBins();
  Float_t resetamax   = GetHistogramRanges()->GetHistoTrackResidualEtaMax();
  Float_t resetamin   = GetHistogramRanges()->GetHistoTrackResidualEtaMin();
  Int_t   nresphibins = GetHistogramRanges()->GetHistoTrackResidualPhiBins();
  Float_t resphimax   = GetHistogramRanges()->GetHistoTrackResidualPhiMax();
  Float_t resphimin   = GetHistogramRanges()->GetHistoTrackResidualPhiMin();
  
  Int_t   ndedxbins   = GetHistogramRanges()->GetHistodEdxBins();
  Float_t dedxmax     = GetHistogramRanges()->GetHistodEdxMax();
  Float_t dedxmin     = GetHistogramRanges()->GetHistodEdxMin();
  Int_t   nPoverEbins = GetHistogramRanges()->GetHistoEOverPBins();
  Float_t pOverEmax   = GetHistogramRanges()->GetHistoEOverPMax();
  Float_t pOverEmin   = GetHistogramRanges()->GetHistoEOverPMin();
  
  Int_t   nratbins = GetHistogramRanges()->GetHistoRatioBins();
  Float_t ratmin   = GetHistogramRanges()->GetHistoRatioMin() ;
  Float_t ratmax   = GetHistogramRanges()->GetHistoRatioMax() ;  
  Int_t   ndifbins = GetHistogramRanges()->GetHistoEDiffBins();
  Float_t difmin   = GetHistogramRanges()->GetHistoEDiffMin() ;
  Float_t difmax   = GetHistogramRanges()->GetHistoEDiffMax() ;

  Int_t nmultbin   = GetHistogramRanges()->GetHistoTrackMultiplicityBins();
  Int_t multmax    = GetHistogramRanges()->GetHistoTrackMultiplicityMax ();
  Int_t multmin    = GetHistogramRanges()->GetHistoTrackMultiplicityMin ();
  
  Int_t   nsumbin  = GetHistogramRanges()->GetHistoNPtSumBins() ;
  Float_t summin   = GetHistogramRanges()->GetHistoPtSumMin()   ;
  Float_t summax   = GetHistogramRanges()->GetHistoPtSumMax()   ;
  
  // Init the number of modules, set in the class AliCalorimeterUtils
  //
  InitCaloParameters(); // See AliCaloTrackCorrBaseClass
  
  Int_t totalSM = fLastModule-fFirstModule+1;
  
  //printf("N SM %d, first SM %d, last SM %d, total %d\n",fNModules,fFirstModule,fLastModule, totalSM);

  // Cell column-row histograms, see base class for data members setting
  //fNMaxColsFull+2,-1.5,fNMaxColsFull+0.5, fNMaxRowsFull+2,-1.5,fNMaxRowsFull+0.5
  Int_t   ncolcell   = fNMaxColsFull+2;
  Float_t colcellmin = -1.5;
  Float_t colcellmax = fNMaxColsFull+0.5;
  
  Int_t   nrowcell   = fNMaxRowsFullMax-fNMaxRowsFullMin+2;
  Float_t rowcellmin = fNMaxRowsFullMin-1.5;
  Float_t rowcellmax = fNMaxRowsFullMax+0.5;
  
  // Non constant binning (TH3)
  //
  TCustomBinning ptBinning;
  ptBinning.SetMinimum(GetMinPt());
  ptBinning.AddStep(12,1);                            // 2 From 10
  if ( GetMaxPt() > 12 ) ptBinning.AddStep( 20, 2.0); // 4
  if ( GetMaxPt() > 20 ) ptBinning.AddStep( 50, 5.0); // 6
  if ( GetMaxPt() > 50 ) ptBinning.AddStep(100,10.0); // 5 
  if ( GetMaxPt() > 100) ptBinning.AddStep(300,20.0); // 10
  
  TArrayD ptBinsArray;
  ptBinning.CreateBinEdges(ptBinsArray);
  //
  TCustomBinning ssBinning;
  ssBinning.SetMinimum(-0.01);
  ssBinning.AddStep(0.50,0.01);  // 51 
  ssBinning.AddStep(1.00,0.05);  // 10
  ssBinning.AddStep(3.00,0.1);   // 20
  ssBinning.AddStep(5.00,0.25);  // 20
  TArrayD ssBinsArray;
  ssBinning.CreateBinEdges(ssBinsArray);
  //
  TCustomBinning frBinning;
  frBinning.SetMinimum(0);
  frBinning.AddStep(1, 0.02); 
  TArrayD frBinsArray;
  frBinning.CreateBinEdges(frBinsArray);
  //
  TCustomBinning ptBinningAcc;
  ptBinningAcc.SetMinimum(GetMinPt());
  ptBinningAcc.AddStep(25,5);                             // 2-4
  if ( GetMaxPt() > 25 ) ptBinningAcc.AddStep(100, 25.0); // 3 
  if ( GetMaxPt() > 100) ptBinningAcc.AddStep(200, 50.0); // 2
  if ( GetMaxPt() > 200) ptBinningAcc.AddStep(300,100.0); // 1
  TArrayD ptBinsAccArray;
  ptBinningAcc.CreateBinEdges(ptBinsAccArray);
  //
  TCustomBinning etaBinning;
  etaBinning.SetMinimum(etamin);
  etaBinning.AddStep(etamax, (etamax-etamin)/netabins); 
  TArrayD etaBinsArray;
  etaBinning.CreateBinEdges(etaBinsArray);
  //
  TCustomBinning phiBinning;
  phiBinning.SetMinimum(phimin);
  phiBinning.AddStep(phimax, (phimax-phimin)/nphibins); 
  TArrayD phiBinsArray;
  phiBinning.CreateBinEdges(phiBinsArray);
  //
  // Cell column-row histograms, see base class for data members setting
  TCustomBinning rowBinning;
  rowBinning.SetMinimum(rowcellmin-1.5);
  rowBinning.AddStep(rowcellmax+0.5,1); 
  TArrayD rowBinsArray;
  rowBinning.CreateBinEdges(rowBinsArray);
  //
  TCustomBinning colBinning;
  colBinning.SetMinimum(colcellmin-1.5);
  colBinning.AddStep(colcellmax+0.5,1);   
  TArrayD colBinsArray;
  colBinning.CreateBinEdges(colBinsArray);
  //
  TCustomBinning cenBinning;
  cenBinning.SetMinimum(0.0);
  if ( GetNCentrBin() > 0 ) 
    cenBinning.AddStep(100, 100./GetNCentrBin()); 
  else 
    cenBinning.AddStep(100, 100.); 
  TArrayD cenBinsArray;
  cenBinning.CreateBinEdges(cenBinsArray);
  //
  TCustomBinning nlmBinning;
  nlmBinning.SetMinimum(1);
  nlmBinning.AddStep(20, 1); 
  TArrayD nlmBinsArray;
  nlmBinning.CreateBinEdges(nlmBinsArray);
  //  
  TCustomBinning nceBinning;
  nceBinning.SetMinimum(1);
  nceBinning.AddStep(30, 1); 
  if ( nmax > 30 )  nceBinning.AddStep( 50, 2); 
  if ( nmax > 50 )  nceBinning.AddStep(100, 5); 
  if ( nmax > 100 ) nceBinning.AddStep(300, 20); 
  TArrayD nceBinsArray;
  nceBinning.CreateBinEdges(nceBinsArray);
  //  
  //  
  TCustomBinning smBinning;
  smBinning.SetMinimum(fFirstModule-0.5);
  smBinning.AddStep(fLastModule+0.5, 1); 
  TArrayD smBinsArray;
  smBinning.CreateBinEdges(smBinsArray);
  //  
  TCustomBinning resBinning; // track matching residuals
  resBinning.SetMinimum(-0.100);
  resBinning.AddStep(-0.050,0.010);                            
  resBinning.AddStep(-0.025,0.005);                            
  resBinning.AddStep( 0.025,0.001);                            
  resBinning.AddStep( 0.050,0.005);                            
  resBinning.AddStep( 0.100,0.010);                            
  TArrayD resBinsArray;
  resBinning.CreateBinEdges(resBinsArray);
  //
  TCustomBinning eopBinning;
  eopBinning.SetMinimum(pOverEmin);
  eopBinning.AddStep(pOverEmax, (pOverEmax-pOverEmin)/nPoverEbins); 
  TArrayD eopBinsArray;
  frBinning.CreateBinEdges(eopBinsArray);
  
  
  Int_t bin[] = {0,2,4,6,10,15,20,100}; // energy bins for SS studies (remove or move to TH3)
  
  // Init the histograms
  //
  TString cut[] = {"Open","Reader","E","Time","NCells","NLM","Fidutial","Matching","Bad","PID","Embed"};
  Int_t ncuts = fgkNClusterCuts;
  if ( !SelectEmbededSignal() ) ncuts = fgkNClusterCuts-1;
  for (Int_t i = 0; i < ncuts ;  i++)
  {
    fhClusterCutsE[i] = new TH1F(Form("hE_Cut_%d_%s", i, cut[i].Data()),
                                Form("Number of clusters that pass cuts <= %d, %s", i, cut[i].Data()),
                                nptbins,ptmin,ptmax);
    fhClusterCutsE[i]->SetYTitle("d#it{N}/d#it{E} ");
    fhClusterCutsE[i]->SetXTitle("#it{E} (GeV)");
    outputContainer->Add(fhClusterCutsE[i]) ;
    
    fhClusterCutsPt[i] = new TH1F(Form("hPt_Cut_%d_%s", i, cut[i].Data()),
                                Form("Number of clusters that pass cuts <= %d, %s", i, cut[i].Data()),
                                nptbins,ptmin,ptmax);
    fhClusterCutsPt[i]->SetYTitle("d#it{N}/d#it{E} ");
    fhClusterCutsPt[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhClusterCutsPt[i]) ;
  }
  
  if  ( !IsHighMultiplicityAnalysisOn() )
  {
    if ( !fFillOnlyPtHisto )
    {
      fhEClusterSM = new TH2F
      ("hEClusterSM","Raw clusters E and supermodule number",
       nptbins,ptmin,ptmax,
       totalSM,fFirstModule-0.5,fLastModule+0.5);
      fhEClusterSM->SetYTitle("Supermodule number");
      fhEClusterSM->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhEClusterSM) ;
      
      fhEPhotonSM = new TH2F
      ("hEPhotonSM","Selected clusters E and supermodule number",
       nptbins,ptmin,ptmax,
       totalSM,fFirstModule-0.5,fLastModule+0.5);
      fhEPhotonSM->SetYTitle("Supermodule number");
      fhEPhotonSM->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhEPhotonSM) ;
    }
    
    fhPtClusterSM = new TH2F
    ("hPtClusterSM","Raw clusters #it{p}_{T} and supermodule number",
     nptbins,ptmin,ptmax,
     totalSM,fFirstModule-0.5,fLastModule+0.5);
    fhPtClusterSM->SetYTitle("Supermodule number");
    fhPtClusterSM->SetXTitle("#it{E} (GeV)");
    outputContainer->Add(fhPtClusterSM) ;
    
    fhPtPhotonSM = new TH2F
    ("hPtPhotonSM","Selected clusters #it{p}_{T} and supermodule number",
     nptbins,ptmin,ptmax,
     totalSM,fFirstModule-0.5,fLastModule+0.5);
    fhPtPhotonSM->SetYTitle("Supermodule number");
    fhPtPhotonSM->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonSM) ;
  }
  else
  {
    fhPtPhotonCentralitySM = new TH3F
    ("hPtPhotonCentralitySM","Selected clusters #it{p}_{T} and super-module number",
       ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
       smBinsArray.GetSize() - 1,   smBinsArray.GetArray(),
      cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
    fhPtPhotonCentralitySM->SetYTitle("Supermodule number");
    fhPtPhotonCentralitySM->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    fhPtPhotonCentralitySM->SetYTitle("Centrality");
    outputContainer->Add(fhPtPhotonCentralitySM) ;
  }
  
  if(fFillTrackMultHistograms)
  {
//    printf("Bins Mult: n %d, min %d, max %d; Sum: n %d, min %f, max %f\n",
//           nmultbin,multmin,multmax,nsumbin,summin, summax);
    for(Int_t icut = 0; icut < GetReader()->GetTrackMultiplicityNPtCut(); icut++)
    {
      fhPtPhotonNTracks[icut]  = new TH2F 
      (Form("hPtPhotonNTracks_PtCut%d",icut),
       Form("Number of tracks per event with |#eta|<%2.2f and #it{p}_{T} > %2.2f GeV/#it{c}",
            GetReader()->GetTrackMultiplicityEtaCut(),GetReader()->GetTrackMultiplicityPtCut(icut)), 
       nptbins,ptmin,ptmax, nmultbin,multmin,multmax); 
      fhPtPhotonNTracks[icut]->SetYTitle("# of tracks");
      fhPtPhotonNTracks[icut]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhPtPhotonNTracks[icut]);
      
      fhPtPhotonSumPtTracks[icut]  = new TH2F 
      (Form("hPtPhotonSumPtTracks_PtCut%d",icut),
       Form("#Sigma #it{p}_{T} of tracks per event with |#eta|<%2.2f and #it{p}_{T} > %2.2f GeV/#it{c}",
            GetReader()->GetTrackMultiplicityEtaCut(),GetReader()->GetTrackMultiplicityPtCut(icut)), 
       nptbins,ptmin,ptmax, nsumbin,summin,summax); 
      fhPtPhotonSumPtTracks[icut]->SetYTitle("#Sigma #it{p}_{T} (GeV/#it{c})");
      fhPtPhotonSumPtTracks[icut]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhPtPhotonSumPtTracks[icut]);
    }
  }
  
  if ( !IsHighMultiplicityAnalysisOn() )
  {
    fhNCellsE  = new TH2F 
    ("hNCellsE","after all selections",
     nptbins,ptmin,ptmax, nbins,nmin,nmax);
    fhNCellsE->SetXTitle("#it{E} (GeV)");
    fhNCellsE->SetYTitle("#it{n}_{cells}");
    outputContainer->Add(fhNCellsE);
    
    fhNLocMaxE = new TH2F
    ("hNLocMaxE","Number of local maxima in cluster after all selections",
     nptbins,ptmin,ptmax,20,0,20);
    fhNLocMaxE ->SetYTitle("#it{n}_{LM}");
    fhNLocMaxE ->SetXTitle("#it{E} (GeV)");
    outputContainer->Add(fhNLocMaxE) ;
    
    if ( fFillControlClusterContentHisto )
    {
      fhNCellsECluster  = new TH2F 
      ("hNCellsECluster","# of cells in cluster vs E of clusters",
       nptbins,ptmin,ptmax, nbins,nmin,nmax);
      fhNCellsECluster->SetXTitle("#it{E} (GeV)");
      fhNCellsECluster->SetYTitle("#it{n}_{cells}");
      outputContainer->Add(fhNCellsECluster);
      
      fhNLocMaxECluster = new TH2F
      ("hNLocMaxECluster","Number of local maxima in cluster",
       nptbins,ptmin,ptmax,20,0,20);
      fhNLocMaxECluster->SetYTitle("#it{n}_{LM}");
      fhNLocMaxECluster->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhNLocMaxECluster) ;
      
      if ( fRejectTrackMatch )
      {
        fhNCellsEClusterNeutral  = new TH2F 
        ("hNCellsEClusterNeutral","# of cells in cluster vs #it{E} of neutral clusters",
         nptbins,ptmin,ptmax, nbins,nmin,nmax);
        fhNCellsEClusterNeutral->SetXTitle("#it{E} (GeV)");
        fhNCellsEClusterNeutral->SetYTitle("#it{n}_{cells}");
        outputContainer->Add(fhNCellsEClusterNeutral);
        
        fhNLocMaxEClusterNeutral = new TH2F
        ("hNLocMaxEClusterNeutral","Number of local maxima in neutral cluster",
         nptbins,ptmin,ptmax,20,0,20);
        fhNLocMaxEClusterNeutral->SetYTitle("#it{n}_{LM}");
        fhNLocMaxEClusterNeutral->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhNLocMaxEClusterNeutral) ;
      }
    }
    
    fhEPhoton  = new TH1F
    ("hEPhoton","Number of #gamma over calorimeter vs energy",
     nptbins,ptmin,ptmax);
    fhEPhoton->SetYTitle("#it{counts}");
    fhEPhoton->SetXTitle("#it{E}_{#gamma}(GeV)");
    outputContainer->Add(fhEPhoton) ;
    
    fhPtPhoton  = new TH1F
    ("hPtPhoton","Number of #gamma over calorimeter vs #it{p}_{T}",
     nptbins,ptmin,ptmax);
    fhPtPhoton->SetYTitle("#it{counts}");
    fhPtPhoton->SetXTitle("p_{T #gamma}(GeV/#it{c})");
    outputContainer->Add(fhPtPhoton) ;
    
    fhTimePt  = new TH2F 
    ("hTimePt","time of cluster vs pT of clusters", 
     nptbins,ptmin,ptmax, ntimebins,timemin,timemax);
    fhTimePt->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    fhTimePt->SetYTitle("#it{time} (ns)");
    outputContainer->Add(fhTimePt);
  }
  else
  {
    fhPtCentralityPhoton  = new TH2F
    ("hPtCentralityPhoton","centrality vs #it{p}_{T}",
     nptbins,ptmin,ptmax, 100,0,100);
    fhPtCentralityPhoton->SetYTitle("Centrality");
    fhPtCentralityPhoton->SetXTitle("#it{p}_{T}(GeV/#it{c})");
    outputContainer->Add(fhPtCentralityPhoton) ;
    
    fhPtEventPlanePhoton  = new TH2F
    ("hPtEventPlanePhoton","centrality vs #it{p}_{T}",
     nptbins,ptmin,ptmax, 100,0,TMath::Pi());
    fhPtEventPlanePhoton->SetYTitle("Event plane angle (rad)");
    fhPtEventPlanePhoton->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtEventPlanePhoton) ;
    
    fhNCellsCentralityE  = new TH3F 
    ("hNCellsCentralityE","after all selections",
      ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
     nceBinsArray.GetSize() - 1,  nceBinsArray.GetArray(),
     cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
    fhNCellsCentralityE->SetXTitle("#it{E} (GeV)");
    fhNCellsCentralityE->SetYTitle("#it{n}_{cells}");
    fhNCellsCentralityE->SetZTitle("Centrality");
    outputContainer->Add(fhNCellsCentralityE);
     
    fhNLocMaxCentralityE = new TH3F
    ("hNLocMaxCentralityE","Number of local maxima in cluster after all selections",
      ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
     nlmBinsArray.GetSize() - 1,  nlmBinsArray.GetArray(),
     cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
     fhNLocMaxCentralityE ->SetYTitle("#it{n}_{LM}");
     fhNLocMaxCentralityE ->SetXTitle("#it{E} (GeV)");
     fhNLocMaxCentralityE->SetZTitle("Centrality");
     outputContainer->Add(fhNLocMaxCentralityE) ;
     
     if ( fFillControlClusterContentHisto )
     {
       fhNCellsCentralityECluster  = new TH3F 
       ("hNCellsCentralityECluster","# of cells in cluster vs E of clusters",
         ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
        nceBinsArray.GetSize() - 1,  nceBinsArray.GetArray(),
        cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
       fhNCellsCentralityECluster->SetXTitle("#it{E} (GeV)");
       fhNCellsCentralityECluster->SetYTitle("#it{n}_{cells}");
       fhNCellsCentralityECluster->SetZTitle("Centrality");
       outputContainer->Add(fhNCellsCentralityECluster);
       
       fhNLocMaxCentralityECluster = new TH3F
       ("hNLocMaxCentralityECluster","Number of local maxima in cluster",
         ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
        nlmBinsArray.GetSize() - 1,  nlmBinsArray.GetArray(),
        cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
       fhNLocMaxCentralityECluster->SetYTitle("#it{n}_{LM}");
       fhNLocMaxCentralityECluster->SetXTitle("#it{E} (GeV)");
       fhNLocMaxCentralityECluster->SetZTitle("Centrality");
       outputContainer->Add(fhNLocMaxCentralityECluster) ;
       
       if ( fRejectTrackMatch )
       {
         fhNCellsCentralityEClusterNeutral  = new TH3F 
         ("hNCellsCentralityEClusterNeutral","# of cells in cluster vs #it{E} of neutral clusters",
            ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
          nceBinsArray.GetSize() - 1,  nceBinsArray.GetArray(),
          cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
         fhNCellsCentralityEClusterNeutral->SetXTitle("#it{E} (GeV)");
         fhNCellsCentralityEClusterNeutral->SetYTitle("#it{n}_{cells}");
         fhNCellsCentralityEClusterNeutral->SetZTitle("Centrality");
         outputContainer->Add(fhNCellsCentralityEClusterNeutral);
         
         fhNLocMaxCentralityEClusterNeutral = new TH3F
         ("hNLocMaxCentralityEClusterNeutral","Number of local maxima in neutral cluster",
           ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
          nlmBinsArray.GetSize() - 1,  nlmBinsArray.GetArray(),
          cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
         fhNLocMaxCentralityEClusterNeutral->SetYTitle("#it{n}_{LM}");
         fhNLocMaxCentralityEClusterNeutral->SetXTitle("#it{E} (GeV)");
         fhNLocMaxCentralityEClusterNeutral->SetZTitle("Centrality");
         outputContainer->Add(fhNLocMaxCentralityEClusterNeutral) ;
       }
     }
  }
  
  if(!fFillOnlySimpleSSHisto && fFillSSHistograms)
  {
    fhMaxCellDiffClusterE  = new TH2F 
    ("hMaxCellDiffClusterE","energy vs difference of cluster energy - max cell energy / cluster energy, good clusters",
     nptbins,ptmin,ptmax, 500,0,1.);
    fhMaxCellDiffClusterE->SetXTitle("#it{E}_{cluster} (GeV) ");
    fhMaxCellDiffClusterE->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
    outputContainer->Add(fhMaxCellDiffClusterE);
  }
  
  fhCellsE  = new TH2F 
  ("hCellsE","energy of cells in cluster vs #it{E} of clusters", 
   nptbins,ptmin,ptmax, nptbins*2,ptmin,ptmax);
  fhCellsE->SetXTitle("#it{E}_{cluster} (GeV)");
  fhCellsE->SetYTitle("#it{E}_{cell} (GeV)");
  outputContainer->Add(fhCellsE);
  
  fhEtaPhi  = new TH2F
  ("hEtaPhi","cluster,#it{E} > 0.5 GeV, #eta vs #varphi",
   netabins,etamin,etamax,nphibins,phimin,phimax);
  fhEtaPhi->SetYTitle("#varphi (rad)");
  fhEtaPhi->SetXTitle("#eta");
  outputContainer->Add(fhEtaPhi) ;
  
  fhPhiPhoton  = new TH2F
  ("hPhiPhoton","#varphi_{#gamma} vs #it{p}_{T}",
   nptbins,ptmin,ptmax,nphibins,phimin,phimax);
  fhPhiPhoton->SetYTitle("#varphi (rad)");
  fhPhiPhoton->SetXTitle("p_{T #gamma} (GeV/#it{c})");
  outputContainer->Add(fhPhiPhoton) ;
  
  fhEtaPhoton  = new TH2F
  ("hEtaPhoton","#eta_{#gamma} vs #it{p}_{T}",
   nptbins,ptmin,ptmax,netabins,etamin,etamax);
  fhEtaPhoton->SetYTitle("#eta");
  fhEtaPhoton->SetXTitle("p_{T #gamma} (GeV/#it{c})");
  outputContainer->Add(fhEtaPhoton) ;
  
  if ( !fFillEBinAcceptanceHisto )
  {
    fhEtaPhiPhoton  = new TH2F
    ("hEtaPhiPhoton","#eta vs #varphi",
     netabins,etamin,etamax,nphibins,phimin,phimax);
    fhEtaPhiPhoton->SetYTitle("#varphi (rad)");
    fhEtaPhiPhoton->SetXTitle("#eta");
    outputContainer->Add(fhEtaPhiPhoton) ;
    if ( GetMinPt() < 0.5 )
    {
      fhEtaPhi05Photon  = new TH2F
      ("hEtaPhi05Photon","#eta vs #varphi, E < 0.5",
       netabins,etamin,etamax,nphibins,phimin,phimax);
      fhEtaPhi05Photon->SetYTitle("#varphi (rad)");
      fhEtaPhi05Photon->SetXTitle("#eta");
      outputContainer->Add(fhEtaPhi05Photon) ;
    }
  }
  else 
  {
    fhEnergyEtaPhi = new TH3F
    ("hEnergyEtaPhi", "cluster #eta vs #varphi vs energy",
     ptBinsAccArray.GetSize() - 1,  ptBinsAccArray.GetArray(),
     etaBinsArray  .GetSize() - 1,  etaBinsArray  .GetArray(),      
     phiBinsArray  .GetSize() - 1,  phiBinsArray  .GetArray());
    fhEnergyEtaPhi->SetZTitle("#varphi (rad)");
    fhEnergyEtaPhi->SetYTitle("#eta");
    fhEnergyEtaPhi->SetXTitle("#it{E} (GeV)");
    outputContainer->Add(fhEnergyEtaPhi) ;
    
    fhEnergyColRow = new TH3F
    ("hEnergyColRow","cluster max E cell column vs row vs energy",
     ptBinsAccArray.GetSize() - 1,  ptBinsAccArray.GetArray(),
     colBinsArray.GetSize() - 1, colBinsArray.GetArray(), 
     rowBinsArray.GetSize() - 1, rowBinsArray.GetArray()) ;    
    fhEnergyColRow->SetZTitle("row");
    fhEnergyColRow->SetYTitle("column");
    fhEnergyColRow->SetXTitle("#it{E} (GeV)");
    outputContainer->Add(fhEnergyColRow) ;
    
    fhEnergyEtaPhiPID = new TH3F
    ("hEnergyEtaPhi_PID","cluster #eta vs #varphi vs energy",
     ptBinsAccArray.GetSize() - 1,  ptBinsAccArray.GetArray(),
     etaBinsArray  .GetSize() - 1,  etaBinsArray  .GetArray(),      
     phiBinsArray  .GetSize() - 1,  phiBinsArray  .GetArray());
    fhEnergyEtaPhiPID->SetZTitle("#varphi (rad)");
    fhEnergyEtaPhiPID->SetYTitle("#eta");
    fhEnergyEtaPhiPID->SetXTitle("#it{E} (GeV)");
    outputContainer->Add(fhEnergyEtaPhiPID) ;
    
    fhEnergyColRowPID = new TH3F
    ("hEnergyColRow_PID","cluster max E cell column vs row vs energy",
     ptBinsAccArray.GetSize() - 1,  ptBinsAccArray.GetArray(),
     colBinsArray.GetSize() - 1, colBinsArray.GetArray(), 
     rowBinsArray.GetSize() - 1, rowBinsArray.GetArray()) ;  
    fhEnergyColRowPID->SetZTitle("row");
    fhEnergyColRowPID->SetYTitle("column");
    fhEnergyColRowPID->SetXTitle("#it{E} (GeV)");
    outputContainer->Add(fhEnergyColRowPID) ;
  }

  TString mcPartLabels[] = {"#gamma","#pi^{0}, #eta (#gamma#gamma)","e^{#pm}","#mu^{#pm}",
    "#pi^{#pm}","k^{#pm}","p","#bar{p}","n","#bar{n}","Other"};
  TString cutCase[] = {"Raw","BeforeTM","AfterTM","Final"};
  if ( IsDataMC() )
  {
    for(Int_t i = 0; i < 4; i++)
    {
      fhMCParticle[i]  = new TH2F
      (Form("hMCParticle%s",cutCase[i].Data()),
       Form("Origin of particle vs energy %s",cutCase[i].Data()),
       nptbins,ptmin,ptmax,11,0,11);
      fhMCParticle[i]->SetXTitle("#it{E} (GeV)");
      //fhMCParticle[i]->SetYTitle("Particle type");
      for(Int_t imcpart = 1; imcpart < 12; imcpart++)
        fhMCParticle[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
      
      outputContainer->Add(fhMCParticle[i]);
      
      fhMCParticleConverted[i]  = new TH2F
      (Form("hMCParticle%s_Converted",cutCase[i].Data()),
       Form("Origin of particle vs energy %s, suffer conversion",cutCase[i].Data()),
       nptbins,ptmin,ptmax,11,0,11);
      fhMCParticleConverted[i]->SetXTitle("#it{E} (GeV)");
      //fhMCParticle[i]->SetYTitle("Particle type");
      for(Int_t imcpart = 1; imcpart < 12; imcpart++)
        fhMCParticleConverted[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
      
      outputContainer->Add(fhMCParticleConverted[i]);
      
      if ( fCheckOverlaps )
      {
        fhMCParticleVsNOverlaps[i]  = new TH3F
        (Form("hMCParticle%s_NOverlaps",cutCase[i].Data()),
         Form("Origin of particle vs energy %s vs number of particle overlaps",cutCase[i].Data()),
         nptbins,ptmin,ptmax,11,0,11,10,0,10);
        fhMCParticleVsNOverlaps[i]->SetXTitle("#it{E} (GeV)");
        fhMCParticleVsNOverlaps[i]->SetZTitle("#it{n}_{overlaps}");
        //fhMCParticle[i]->SetYTitle("Particle type");
        for(Int_t imcpart = 1; imcpart < 12; imcpart++)
          fhMCParticleVsNOverlaps[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
        
        outputContainer->Add(fhMCParticleVsNOverlaps[i]);
      }
      
      fhMCParticleVsErecEgen[i]  = new TH3F
      (Form("hMCParticle%s_ErecEgenRat",cutCase[i].Data()),
       Form("Origin of particle vs energy %s vs #it{E}_{rec}/#it{E}_{gen}",cutCase[i].Data()),
       nptbins,ptmin,ptmax,11,0,11,100,0,2);
      fhMCParticleVsErecEgen[i]->SetXTitle("#it{E}_{rec} (GeV)");
      fhMCParticleVsErecEgen[i]->SetZTitle("#it{E}_{rec}/#it{E}_{gen}");
      //fhMCParticle[i]->SetYTitle("Particle type");
      for(Int_t imcpart = 1; imcpart < 12; imcpart++)
        fhMCParticleVsErecEgen[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
      
      outputContainer->Add(fhMCParticleVsErecEgen[i]);
    }
  }
  
  if ( fFillControlClusterContentHisto )
  {
    if ( !IsHighMultiplicityAnalysisOn() )
    {
      fhLam0ECluster  = new TH2F ("hLam0ECluster","#sigma^{2}_{long} vs E", 
                                  nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0ECluster->SetYTitle("#sigma^{2}_{long}");
      fhLam0ECluster->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam0ECluster);
      
      if ( fRejectTrackMatch )
      {
        fhLam0EClusterNeutral  = new TH2F ("hLam0EClusterNeutral","#sigma^{2}_{long} vs E",
                                           nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhLam0EClusterNeutral->SetYTitle("#sigma^{2}_{long}");
        fhLam0EClusterNeutral->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhLam0EClusterNeutral);
      }
    }
    else
    {
      fhLam0CentralityECluster  = new TH3F 
      ("hLam0CentralityECluster","#sigma^{2}_{long} vs E", 
        ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
        ssBinsArray.GetSize() - 1,   ssBinsArray.GetArray(),
       cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
      fhLam0CentralityECluster->SetYTitle("#sigma^{2}_{long}");
      fhLam0CentralityECluster->SetXTitle("#it{E} (GeV)");
      fhLam0CentralityECluster->SetZTitle("Centrality");
      outputContainer->Add(fhLam0CentralityECluster);
      
      if ( fRejectTrackMatch )
      {
        fhLam0CentralityEClusterNeutral  = new TH3F 
        ("hLam0CentralityEClusterNeutral","#sigma^{2}_{long} vs E",
          ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
          ssBinsArray.GetSize() - 1,   ssBinsArray.GetArray(),
         cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
        fhLam0CentralityEClusterNeutral->SetYTitle("#sigma^{2}_{long}");
        fhLam0CentralityEClusterNeutral->SetXTitle("#it{E} (GeV)");
        fhLam0CentralityEClusterNeutral->SetZTitle("Centrality");
        outputContainer->Add(fhLam0CentralityEClusterNeutral);
      }
    }
  }
  
  //S hower shape
  if ( fFillSSHistograms )
  {
    if ( !IsHighMultiplicityAnalysisOn() )
    {
      fhLam0E  = new TH2F ("hLam0E","#sigma^{2}_{long} vs E", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0E->SetYTitle("#sigma^{2}_{long}");
      fhLam0E->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam0E);
      
      fhLam0Pt  = new TH2F ("hLam0Pt","#sigma^{2}_{long} vs #it{p}_{T}", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0Pt->SetYTitle("#sigma^{2}_{long}");
      fhLam0Pt->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0Pt);
      
      fhLam1E  = new TH2F ("hLam1E","#sigma^{2}_{short} vs E", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam1E->SetYTitle("#sigma^{2}_{short}");
      fhLam1E->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam1E);
      
      fhLam1Pt  = new TH2F ("hLam1Pt","#sigma^{2}_{short} vs E", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam1Pt->SetYTitle("#sigma^{2}_{short}");
      fhLam1Pt->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam1Pt);
      
      if ( fFillSSNLocMaxHisto && fFillControlClusterContentHisto )
       {
         fhLam0NLocMaxECluster  = new TH3F 
         ("hLam0NLocMaxECluster",
          "#sigma^{2}_{long} vs #it{n}_{LM} vs #it{p}_{T}",
           ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
           ssBinsArray.GetSize() - 1,   ssBinsArray.GetArray(),
          nlmBinsArray.GetSize() - 1,  nlmBinsArray.GetArray());
         fhLam0NLocMaxECluster->SetZTitle("#it{n}_{LM}");
         fhLam0NLocMaxECluster->SetYTitle("#sigma^{2}_{long}");
         fhLam0NLocMaxECluster->SetXTitle("#it{E} (GeV/#it{c})");
         outputContainer->Add(fhLam0NLocMaxECluster);
         
         if ( fRejectTrackMatch )
         {
           fhLam0NLocMaxEClusterNeutral  = new TH3F 
           ("hLam0NLocMaxEClusterNeutral",
            "#sigma^{2}_{long} vs #it{n}_{LM} vs #it{p}_{T}, after track matching",
             ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
             ssBinsArray.GetSize() - 1,   ssBinsArray.GetArray(),
            nlmBinsArray.GetSize() - 1,  nlmBinsArray.GetArray());
           fhLam0NLocMaxEClusterNeutral->SetZTitle("#it{n}_{LM}");
           fhLam0NLocMaxEClusterNeutral->SetYTitle("#sigma^{2}_{long}");
           fhLam0NLocMaxEClusterNeutral->SetXTitle("#it{E} (GeV/#it{c})");
           outputContainer->Add(fhLam0NLocMaxEClusterNeutral);
         }
       }
    }
    else
    {
      fhLam0CentralityE  = new TH3F 
      ("hLam0CentralityE","#sigma^{2}_{long} vs #it{p}_{T}", 
        ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
        ssBinsArray.GetSize() - 1,   ssBinsArray.GetArray(),
       cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
      fhLam0CentralityE->SetYTitle("#sigma^{2}_{long}");
      fhLam0CentralityE->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      fhLam0CentralityE->SetZTitle("Centrality");
      outputContainer->Add(fhLam0CentralityE);
      
      if ( fFillSSNLocMaxHisto && fFillControlClusterContentHisto )
       {
         fhLam0NLocMaxEClusterPerCen = new TH3F*[GetNCentrBin()] ;
         if ( fRejectTrackMatch )
            fhLam0NLocMaxEClusterNeutralPerCen = new TH3F*[GetNCentrBin()] ;
         
         for(Int_t icent = 0; icent < GetNCentrBin(); icent++)
         {
           fhLam0NLocMaxEClusterPerCen[icent]  = new TH3F 
           (Form("hLam0NLocMaxECluster_Cent%d",icent),
            Form("#sigma^{2}_{long} vs #it{n}_{LM} vs #it{p}_{T}, cent bin %d",icent),
             ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
             ssBinsArray.GetSize() - 1,   ssBinsArray.GetArray(),
            nlmBinsArray.GetSize() - 1,  nlmBinsArray.GetArray());
           fhLam0NLocMaxEClusterPerCen[icent]->SetZTitle("#it{n}_{LM}");
           fhLam0NLocMaxEClusterPerCen[icent]->SetYTitle("#sigma^{2}_{long}");
           fhLam0NLocMaxEClusterPerCen[icent]->SetXTitle("#it{E} (GeV/#it{c})");
           outputContainer->Add(fhLam0NLocMaxEClusterPerCen[icent]);
           
           if ( fRejectTrackMatch )
           {
             fhLam0NLocMaxEClusterNeutralPerCen[icent]  = new TH3F 
             (Form("hLam0NLocMaxEClusterNeutral_Cent%d",icent),
              Form("#sigma^{2}_{long} vs #it{n}_{LM} vs #it{p}_{T}, after track matching, cent bin %d",icent),
               ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
               ssBinsArray.GetSize() - 1,   ssBinsArray.GetArray(),
              nlmBinsArray.GetSize() - 1,  nlmBinsArray.GetArray());
             fhLam0NLocMaxEClusterNeutralPerCen[icent]->SetZTitle("#it{n}_{LM}");
             fhLam0NLocMaxEClusterNeutralPerCen[icent]->SetYTitle("#sigma^{2}_{long}");
             fhLam0NLocMaxEClusterNeutralPerCen[icent]->SetXTitle("#it{E} (GeV/#it{c})");
             outputContainer->Add(fhLam0NLocMaxEClusterNeutralPerCen[icent]);
           }
         }
       }
    }
    
    if ( fFillSSPerSMHistograms )
    {
      for(Int_t ism = 0; ism < fNModules; ism++)
      {
        if(ism < fFirstModule || ism > fLastModule) continue;
        fhLam0PerSM[ism] = new TH2F
        (Form("hLam0_SM%d",ism),
         Form("#it{p}_{T} vs #sigma^{2}_{long} in SM %d",ism),
         nptbins,ptmin,ptmax,40,0,0.4);
        fhLam0PerSM[ism]->SetYTitle("#sigma^{2}_{long}");
        fhLam0PerSM[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhLam0PerSM[ism]) ;             
        
        fhLam1PerSM[ism] = new TH2F
        (Form("hLam1_SM%d",ism),
         Form("#it{p}_{T} vs #sigma^{2}_{short} in SM %d",ism),
         nptbins,ptmin,ptmax,40,0,0.4);
        fhLam1PerSM[ism]->SetYTitle("#sigma^{2}_{short}");
        fhLam1PerSM[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhLam1PerSM[ism]) ;   
      }
    }
    
    if ( !fFillOnlySimpleSSHisto )
    {
      fhDispE  = new TH2F ("hDispE"," dispersion^{2} vs E", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhDispE->SetYTitle("D^{2}");
      fhDispE->SetXTitle("#it{E} (GeV) ");
      outputContainer->Add(fhDispE);
      
      fhDispPt  = new TH2F ("hDispPt"," dispersion^{2} vs #it{p}_{T}", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhDispPt->SetYTitle("D^{2}");
      fhDispPt->SetXTitle("#it{p}_{T} (GeV/#it{c}) ");
      outputContainer->Add(fhDispPt);
    }
    
    if ( !fRejectTrackMatch )
    {
      fhLam0ETM  = new TH2F ("hLam0ETM","#sigma^{2}_{long} vs E, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #varphi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0ETM->SetYTitle("#sigma^{2}_{long}");
      fhLam0ETM->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam0ETM);

      fhLam0PtTM  = new TH2F ("hLam0PtTM","#sigma^{2}_{long} vs #it{p}_{T}, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #varphi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0PtTM->SetYTitle("#sigma^{2}_{long}");
      fhLam0PtTM->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PtTM);
      
      fhLam1ETM  = new TH2F ("hLam1ETM","#sigma^{2}_{short} vs E, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #varphi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam1ETM->SetYTitle("#sigma^{2}_{short}");
      fhLam1ETM->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam1ETM);
      
      if(!fFillOnlySimpleSSHisto && fFillSSHistograms)
      {
        fhDispETM  = new TH2F ("hDispETM"," dispersion^{2} vs E, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #varphi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhDispETM->SetYTitle("D^{2}");
        fhDispETM->SetXTitle("#it{E} (GeV) ");
        outputContainer->Add(fhDispETM);
      }
    }
    
    if(GetCalorimeter() == kEMCAL &&  GetFirstSMCoveredByTRD() >= 0)
    {
      fhLam0ETRD  = new TH2F ("hLam0ETRD","#sigma^{2}_{long} vs E, EMCAL SM covered by TRD", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0ETRD->SetYTitle("#sigma^{2}_{long}");
      fhLam0ETRD->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam0ETRD);
      
      fhLam0PtTRD  = new TH2F ("hLam0PtTRD","#sigma^{2}_{long} vs #it{p}_{T}, EMCAL SM covered by TRD", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0PtTRD->SetYTitle("#sigma^{2}_{long}");
      fhLam0PtTRD->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PtTRD);
      
      fhLam1ETRD  = new TH2F ("hLam1ETRD","#sigma^{2}_{short} vs E, EMCAL SM covered by TRD", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam1ETRD->SetYTitle("#sigma^{2}_{short}");
      fhLam1ETRD->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam1ETRD);
      
      if(fFillSSHistograms && !fFillOnlySimpleSSHisto)
      {
        fhDispETRD  = new TH2F ("hDispETRD"," dispersion^{2} vs E, EMCAL SM covered by TRD", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhDispETRD->SetYTitle("Dispersion^{2}");
        fhDispETRD->SetXTitle("#it{E} (GeV) ");
        outputContainer->Add(fhDispETRD);
      }
      
      if(!fRejectTrackMatch &&  GetFirstSMCoveredByTRD() >=0 )
      {
        fhLam0ETMTRD  = new TH2F ("hLam0ETMTRD","#sigma^{2}_{long} vs E, EMCAL SM covered by TRD, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #varphi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhLam0ETMTRD->SetYTitle("#sigma^{2}_{long}");
        fhLam0ETMTRD->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhLam0ETMTRD);
        
        fhLam0PtTMTRD  = new TH2F ("hLam0PtTMTRD","#sigma^{2}_{long} vs #it{p}_{T}, EMCAL SM covered by TRD, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #varphi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhLam0PtTMTRD->SetYTitle("#sigma^{2}_{long}");
        fhLam0PtTMTRD->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhLam0PtTMTRD);
        
        fhLam1ETMTRD  = new TH2F ("hLam1ETMTRD","#sigma^{2}_{short} vs E, EMCAL SM covered by TRD, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #varphi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhLam1ETMTRD->SetYTitle("#sigma^{2}_{short}");
        fhLam1ETMTRD->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhLam1ETMTRD);
        
        if(fFillSSHistograms && !fFillOnlySimpleSSHisto)
        {
          fhDispETMTRD  = new TH2F ("hDispETMTRD"," dispersion^{2} vs E, EMCAL SM covered by TRD, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #varphi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhDispETMTRD->SetYTitle("Dispersion^{2}");
          fhDispETMTRD->SetXTitle("#it{E} (GeV) ");
          outputContainer->Add(fhDispETMTRD);
        }
      }
    }
    
    if(!fFillOnlySimpleSSHisto)
    {
      fhNCellsLam0LowE  = new TH2F ("hNCellsLam0LowE","N_{cells} in cluster vs #sigma^{2}_{long}, E < 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsLam0LowE->SetXTitle("N_{cells}");
      fhNCellsLam0LowE->SetYTitle("#sigma^{2}_{long}");
      outputContainer->Add(fhNCellsLam0LowE);
      
      fhNCellsLam0HighE  = new TH2F ("hNCellsLam0HighE","N_{cells} in cluster vs #sigma^{2}_{long}, #it{E} > 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsLam0HighE->SetXTitle("N_{cells}");
      fhNCellsLam0HighE->SetYTitle("#sigma^{2}_{long}");
      outputContainer->Add(fhNCellsLam0HighE);
      
      fhNCellsLam1LowE  = new TH2F ("hNCellsLam1LowE","N_{cells} in cluster vs #sigma^{2}_{short}, E < 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsLam1LowE->SetXTitle("N_{cells}");
      fhNCellsLam1LowE->SetYTitle("#sigma^{2}_{long}");
      outputContainer->Add(fhNCellsLam1LowE);
      
      fhNCellsLam1HighE  = new TH2F ("hNCellsLam1HighE","N_{cells} in cluster vs #sigma^{2}_{short}, #it{E} > 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsLam1HighE->SetXTitle("N_{cells}");
      fhNCellsLam1HighE->SetYTitle("#sigma^{2}_{long}");
      outputContainer->Add(fhNCellsLam1HighE);
      
      fhNCellsDispLowE  = new TH2F ("hNCellsDispLowE","N_{cells} in cluster vs dispersion^{2}, E < 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsDispLowE->SetXTitle("N_{cells}");
      fhNCellsDispLowE->SetYTitle("D^{2}");
      outputContainer->Add(fhNCellsDispLowE);
      
      fhNCellsDispHighE  = new TH2F ("hNCellsDispHighE","N_{cells} in cluster vs dispersion^{2}, E < 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsDispHighE->SetXTitle("N_{cells}");
      fhNCellsDispHighE->SetYTitle("D^{2}");
      outputContainer->Add(fhNCellsDispHighE);
      
      fhEtaLam0LowE  = new TH2F ("hEtaLam0LowE","#eta vs #sigma^{2}_{long}, E < 2 GeV", netabins,etamin,etamax, ssbins,ssmin,ssmax);
      fhEtaLam0LowE->SetYTitle("#sigma^{2}_{long}");
      fhEtaLam0LowE->SetXTitle("#eta");
      outputContainer->Add(fhEtaLam0LowE);
      
      fhPhiLam0LowE  = new TH2F ("hPhiLam0LowE","#varphi vs #sigma^{2}_{long}, E < 2 GeV", nphibins,phimin,phimax, ssbins,ssmin,ssmax);
      fhPhiLam0LowE->SetYTitle("#sigma^{2}_{long}");
      fhPhiLam0LowE->SetXTitle("#varphi (rad)");
      outputContainer->Add(fhPhiLam0LowE);
      
      fhEtaLam0HighE  = new TH2F ("hEtaLam0HighE","#eta vs #sigma^{2}_{long}, #it{E} > 2 GeV", netabins,etamin,etamax, ssbins,ssmin,ssmax);
      fhEtaLam0HighE->SetYTitle("#sigma^{2}_{long}");
      fhEtaLam0HighE->SetXTitle("#eta");
      outputContainer->Add(fhEtaLam0HighE);
      
      fhPhiLam0HighE  = new TH2F ("hPhiLam0HighE","#varphi vs #sigma^{2}_{long}, #it{E} > 2 GeV", nphibins,phimin,phimax, ssbins,ssmin,ssmax);
      fhPhiLam0HighE->SetYTitle("#sigma^{2}_{long}");
      fhPhiLam0HighE->SetXTitle("#varphi (rad)");
      outputContainer->Add(fhPhiLam0HighE);
      
      fhLam1Lam0LowE  = new TH2F ("hLam1Lam0LowE","#sigma^{2}_{long} vs #sigma^{2}_{short} in cluster of E < 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhLam1Lam0LowE->SetYTitle("#sigma^{2}_{long}");
      fhLam1Lam0LowE->SetXTitle("#sigma^{2}_{short}");
      outputContainer->Add(fhLam1Lam0LowE);
      
      fhLam1Lam0HighE  = new TH2F ("hLam1Lam0HighE","#sigma^{2}_{long} vs #sigma^{2}_{short} in cluster of #it{E} > 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhLam1Lam0HighE->SetYTitle("#sigma^{2}_{long}");
      fhLam1Lam0HighE->SetXTitle("#sigma^{2}_{short}");
      outputContainer->Add(fhLam1Lam0HighE);
      
      fhLam0DispLowE  = new TH2F ("hLam0DispLowE","#sigma^{2}_{long} vs dispersion^{2} in cluster of E < 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhLam0DispLowE->SetXTitle("#sigma^{2}_{long}");
      fhLam0DispLowE->SetYTitle("D^{2}");
      outputContainer->Add(fhLam0DispLowE);
      
      fhLam0DispHighE  = new TH2F ("hLam0DispHighE","#sigma^{2}_{long} vs dispersion^{2} in cluster of #it{E} > 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhLam0DispHighE->SetXTitle("#sigma^{2}_{long}");
      fhLam0DispHighE->SetYTitle("D^{2}");
      outputContainer->Add(fhLam0DispHighE);
      
      fhDispLam1LowE  = new TH2F ("hDispLam1LowE","Dispersion^{2} vs #sigma^{2}_{short} in cluster of E < 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhDispLam1LowE->SetXTitle("D^{2}");
      fhDispLam1LowE->SetYTitle("#sigma^{2}_{short}");
      outputContainer->Add(fhDispLam1LowE);
      
      fhDispLam1HighE  = new TH2F ("hDispLam1HighE","Dispersion^{2} vs #lambda_{1^{2}} in cluster of #it{E} > 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhDispLam1HighE->SetXTitle("D^{2}");
      fhDispLam1HighE->SetYTitle("#sigma^{2}_{short}");
      outputContainer->Add(fhDispLam1HighE);
      
      if(GetCalorimeter() == kEMCAL)
      {
        fhDispEtaE  = new TH2F ("hDispEtaE","#sigma^{2}_{#eta #eta} = #Sigma w_{i}(#eta_{i} - <#eta>)^{2}/ #Sigma w_{i} vs E",  nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
        fhDispEtaE->SetXTitle("#it{E} (GeV)");
        fhDispEtaE->SetYTitle("#sigma^{2}_{#eta #eta}");
        outputContainer->Add(fhDispEtaE);
        
        fhDispPhiE  = new TH2F ("hDispPhiE","#sigma^{2}_{#varphi #varphi} = #Sigma w_{i}(#varphi_{i} - <#varphi>)^{2} / #Sigma w_{i} vs E",  nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
        fhDispPhiE->SetXTitle("#it{E} (GeV)");
        fhDispPhiE->SetYTitle("#sigma^{2}_{#varphi #varphi}");
        outputContainer->Add(fhDispPhiE);
        
        fhSumEtaE  = new TH2F ("hSumEtaE","#delta^{2}_{#eta #eta} = #Sigma w_{i}(#eta_{i})^{2} / #Sigma w_{i} - <#eta>^{2} vs E",  nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
        fhSumEtaE->SetXTitle("#it{E} (GeV)");
        fhSumEtaE->SetYTitle("#delta^{2}_{#eta #eta}");
        outputContainer->Add(fhSumEtaE);
        
        fhSumPhiE  = new TH2F ("hSumPhiE","#delta^{2}_{#varphi #varphi} = #Sigma w_{i}(#varphi_{i})^{2}/ #Sigma w_{i} - <#varphi>^{2} vs E",
                               nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
        fhSumPhiE->SetXTitle("#it{E} (GeV)");
        fhSumPhiE->SetYTitle("#delta^{2}_{#varphi #varphi}");
        outputContainer->Add(fhSumPhiE);
        
        fhSumEtaPhiE  = new TH2F ("hSumEtaPhiE","#delta^{2}_{#eta #varphi} = #Sigma w_{i}(#varphi_{i} #eta_{i} ) / #Sigma w_{i} - <#varphi><#eta> vs E",
                                  nptbins,ptmin,ptmax, 2*ssbins,-ssmax,ssmax);
        fhSumEtaPhiE->SetXTitle("#it{E} (GeV)");
        fhSumEtaPhiE->SetYTitle("#delta^{2}_{#eta #varphi}");
        outputContainer->Add(fhSumEtaPhiE);
        
        fhDispEtaPhiDiffE  = new TH2F ("hDispEtaPhiDiffE","#sigma^{2}_{#varphi #varphi} - #sigma^{2}_{#eta #eta} vs E",
                                       nptbins,ptmin,ptmax,200, -10,10);
        fhDispEtaPhiDiffE->SetXTitle("#it{E} (GeV)");
        fhDispEtaPhiDiffE->SetYTitle("#sigma^{2}_{#varphi #varphi}-#sigma^{2}_{#eta #eta}");
        outputContainer->Add(fhDispEtaPhiDiffE);
        
        fhSphericityE  = new TH2F ("hSphericityE","(#sigma^{2}_{#varphi #varphi} - #sigma^{2}_{#eta #eta}) / (#sigma^{2}_{#eta #eta} + #sigma^{2}_{#varphi #varphi}) vs E",
                                   nptbins,ptmin,ptmax, 200, -1,1);
        fhSphericityE->SetXTitle("#it{E} (GeV)");
        fhSphericityE->SetYTitle("s = (#sigma^{2}_{#varphi #varphi} - #sigma^{2}_{#eta #eta}) / (#sigma^{2}_{#eta #eta} + #sigma^{2}_{#varphi #varphi})");
        outputContainer->Add(fhSphericityE);
        
        fhDispSumEtaDiffE  = new TH2F ("hDispSumEtaDiffE","#sigma^{2}_{#eta #eta} - #delta^{2}_{#eta #eta} / average vs E",  nptbins,ptmin,ptmax, 200,-0.01,0.01);
        fhDispSumEtaDiffE->SetXTitle("#it{E} (GeV)");
        fhDispSumEtaDiffE->SetYTitle("#sigma^{2}_{#eta #eta} - #delta^{2}_{#eta #eta} / average");
        outputContainer->Add(fhDispSumEtaDiffE);
        
        fhDispSumPhiDiffE  = new TH2F ("hDispSumPhiDiffE","#sigma^{2}_{#varphi #varphi} - #delta^{2}_{#varphi #varphi} / average vs E",  nptbins,ptmin,ptmax, 200,-0.01,0.01);
        fhDispSumPhiDiffE->SetXTitle("#it{E} (GeV)");
        fhDispSumPhiDiffE->SetYTitle("#sigma^{2}_{#varphi #varphi} - #delta^{2}_{#varphi #varphi} / average");
        outputContainer->Add(fhDispSumPhiDiffE);
        
        for(Int_t i = 0; i < 7; i++)
        {
          fhDispEtaDispPhi[i] = new TH2F (Form("hDispEtaDispPhi_EBin%d",i),Form("#sigma^{2}_{#varphi #varphi} vs #sigma^{2}_{#eta #eta} for %d < E < %d GeV",bin[i],bin[i+1]),
                                          ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
          fhDispEtaDispPhi[i]->SetXTitle("#sigma^{2}_{#eta #eta}");
          fhDispEtaDispPhi[i]->SetYTitle("#sigma^{2}_{#varphi #varphi}");
          outputContainer->Add(fhDispEtaDispPhi[i]);
          
          fhLambda0DispEta[i] = new TH2F (Form("hLambda0DispEta_EBin%d",i),Form("#sigma^{2}_{long} vs #sigma^{2}_{#eta #eta} for %d < E < %d GeV",bin[i],bin[i+1]),
                                          ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
          fhLambda0DispEta[i]->SetXTitle("#sigma^{2}_{long}");
          fhLambda0DispEta[i]->SetYTitle("#sigma^{2}_{#eta #eta}");
          outputContainer->Add(fhLambda0DispEta[i]);
          
          fhLambda0DispPhi[i] = new TH2F (Form("hLambda0DispPhi_EBin%d",i),Form("#sigma^{2}_{long}} vs #sigma^{2}_{#varphi #varphi} for %d < E < %d GeV",bin[i],bin[i+1]),
                                          ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
          fhLambda0DispPhi[i]->SetXTitle("#sigma^{2}_{long}");
          fhLambda0DispPhi[i]->SetYTitle("#sigma^{2}_{#varphi #varphi}");
          outputContainer->Add(fhLambda0DispPhi[i]);
        }
      }
    }
  } // Shower shape
  
  // Track Matching
  
  if ( fFillTMHisto && IsHighMultiplicityAnalysisOn() )
  {   
    fhEOverPCentrality  = new TH3F 
    ("hEOverPCentrality",
     "matched track #it{E}/#it{p} vs cluster #it{E} vs centrality bin",
     ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
     eopBinsArray.GetSize() - 1,  eopBinsArray.GetArray(),
     cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());    
    fhEOverPCentrality->SetXTitle("#it{E}^{cluster} (GeV)");
    fhEOverPCentrality->SetYTitle("#it{E}/#it{p}");
    fhEOverPCentrality->SetZTitle("Centrality");
    outputContainer->Add(fhEOverPCentrality);
    
    fhEOverPCentralityAfterResidualCut  = new TH3F 
    ("hEOverPCentralityAfterResidualCut",
     "matched track #it{E}/#it{p} vs cluster #it{p}_{T}^{track} vs centrality bin, after TM residual cut",
     ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
     eopBinsArray.GetSize() - 1,  eopBinsArray.GetArray(),
     cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());    
    fhEOverPCentralityAfterResidualCut->SetXTitle("#it{p}_{T}^{track} (GeV)/#{c}");
    fhEOverPCentralityAfterResidualCut->SetYTitle("#it{E}/#it{p}");
    fhEOverPCentralityAfterResidualCut->SetZTitle("Centrality");
    outputContainer->Add(fhEOverPCentralityAfterResidualCut);
    
    if ( fFillTMHistoTrackPt )
    {
      fhEOverPCentralityTrackPt  = new TH3F 
      ("hEOverPCentralityTrackOt",
       "matched track #it{E}/#it{p} vs track #it{p}_{T} vs centrality bin",
        ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(), 
       eopBinsArray.GetSize() - 1,  eopBinsArray.GetArray(),
       cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());    
      fhEOverPCentrality->SetXTitle("#it{E}^{cluster} (GeV)");
      fhEOverPCentrality->SetYTitle("#it{E}/#it{p}");
      fhEOverPCentrality->SetZTitle("Centrality");
      outputContainer->Add(fhEOverPCentrality);
      
      fhEOverPCentralityTrackPtAfterResidualCut  = new TH3F 
      ("hEOverPCentralityTrackPtAfterResidualCut",
       "matched track #it{E}/#it{p} vs cluster #it{E} vs centrality bin, after TM residual cut",
        ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
       eopBinsArray.GetSize() - 1,  eopBinsArray.GetArray(),
       cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());    
      fhEOverPCentralityTrackPtAfterResidualCut->SetXTitle("#it{p}_{T}^{track} (GeV)/#{c}");
      fhEOverPCentralityTrackPtAfterResidualCut->SetYTitle("#it{E}/#it{p}");
      fhEOverPCentralityTrackPtAfterResidualCut->SetZTitle("Centrality");
      outputContainer->Add(fhEOverPCentralityTrackPtAfterResidualCut);
    }
    
    fhTrackMatchedDEtaDPhiPosPerCen = new TH3F*[GetNCentrBin()] ;    
    fhTrackMatchedDEtaDPhiPosAfterEOverPCutPerCen = new TH3F*[GetNCentrBin()] ;
    if ( fFillTMHistoTrackPt )
    {
      fhTrackMatchedDEtaDPhiPosTrackPtPerCen = new TH3F*[GetNCentrBin()] ; 
      fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCutPerCen = new TH3F*[GetNCentrBin()] ;
    }
    
    for(Int_t icent = 0; icent < GetNCentrBin(); icent++)
    {
      fhTrackMatchedDEtaDPhiPosPerCen[icent]  = new TH3F
      (Form("hTrackMatchedDEtaDPhiPos_Cen%d",icent),
       Form("#Delta #eta vs #Delta #varphi vs #it{E}_{cluster}, of cluster-positive track, cen %d",icent),
        ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
       resBinsArray.GetSize() - 1,  resBinsArray.GetArray(),
       resBinsArray.GetSize() - 1,  resBinsArray.GetArray());
      fhTrackMatchedDEtaDPhiPosPerCen[icent]->SetZTitle("#Delta #varphi (rad)");
      fhTrackMatchedDEtaDPhiPosPerCen[icent]->SetYTitle("#Delta #eta");
      fhTrackMatchedDEtaDPhiPosPerCen[icent]->SetXTitle("#it{E}_{cluster} (GeV)");
      outputContainer->Add(fhTrackMatchedDEtaDPhiPosPerCen[icent]) ;
            
      fhTrackMatchedDEtaDPhiPosAfterEOverPCutPerCen[icent]  = new TH3F
      (Form("hTrackMatchedDEtaDPhiPosAfterEOverPCut_Cen%d",icent),
       Form("#Delta #eta vs #Delta #varphi vs #it{E}_{cluster}, of cluster-positive track, E/p cut, cen %d",icent),
        ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
       resBinsArray.GetSize() - 1,  resBinsArray.GetArray(),
       resBinsArray.GetSize() - 1,  resBinsArray.GetArray());
      fhTrackMatchedDEtaDPhiPosAfterEOverPCutPerCen[icent]->SetZTitle("#Delta #varphi (rad)");
      fhTrackMatchedDEtaDPhiPosAfterEOverPCutPerCen[icent]->SetYTitle("#Delta #eta");
      fhTrackMatchedDEtaDPhiPosAfterEOverPCutPerCen[icent]->SetXTitle("#it{E}_{cluster} (GeV)");
      outputContainer->Add(fhTrackMatchedDEtaDPhiPosAfterEOverPCutPerCen[icent]) ;
      
      if ( fFillTMHistoTrackPt )
      {
        fhTrackMatchedDEtaDPhiPosTrackPtPerCen[icent]  = new TH3F
        (Form("hTrackMatchedDEtaDPhiPosTrackPt_Cen%d",icent),
         Form("#Delta #eta vs #Delta #varphi vs #it{p}_{T}^{track}, of cluster-positive track, cen %d",icent),
          ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
         resBinsArray.GetSize() - 1,  resBinsArray.GetArray(),
         resBinsArray.GetSize() - 1,  resBinsArray.GetArray());
        fhTrackMatchedDEtaDPhiPosTrackPtPerCen[icent]->SetZTitle("#Delta #varphi (rad)");
        fhTrackMatchedDEtaDPhiPosTrackPtPerCen[icent]->SetYTitle("#Delta #eta");
        fhTrackMatchedDEtaDPhiPosTrackPtPerCen[icent]->SetXTitle("#it{p}_{T}^{track} (GeV/#it{c})");
        outputContainer->Add(fhTrackMatchedDEtaDPhiPosTrackPtPerCen[icent]) ;
        
        fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCutPerCen[icent]  = new TH3F
        (Form("hTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCut_Cen%d",icent),
         Form("#Delta #eta vs #Delta #varphi vs #it{p}_{T}^{track}, of cluster-positive track, E/p cut, cen %d",icent),
          ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
         resBinsArray.GetSize() - 1,  resBinsArray.GetArray(),
         resBinsArray.GetSize() - 1,  resBinsArray.GetArray());
        fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCutPerCen[icent]->SetZTitle("#Delta #varphi (rad)");
        fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCutPerCen[icent]->SetYTitle("#Delta #eta");
        fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCutPerCen[icent]->SetXTitle("#it{p}_{T}^{track} (GeV/#it{c})");
        outputContainer->Add(fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCutPerCen[icent]) ;
      }
    }
  } // centrality histograms for track matching
  else if ( fFillTMHisto )
  {
    TString cutTM [] = {"NoCut",""};
    
    Int_t nTM = 2;
    if ( !fFillTMHistoAfterCut ) nTM = 1;
    
    for(Int_t i = 0; i < nTM; i++)
    {
      fhTrackMatchedDEtaDPhiPos[i]  = new TH3F
      (Form("hTrackMatchedDEtaDPhiPos%s",cutTM[i].Data()),
       Form("#Delta #eta vs #Delta #varphi vs #it{E}_{cluster}, of cluster-positive track, %s",cutTM[i].Data()),
        ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
       resBinsArray.GetSize() - 1,  resBinsArray.GetArray(),
       resBinsArray.GetSize() - 1,  resBinsArray.GetArray());
      fhTrackMatchedDEtaDPhiPos[i]->SetZTitle("#Delta #varphi (rad)");
      fhTrackMatchedDEtaDPhiPos[i]->SetYTitle("#Delta #eta");
      fhTrackMatchedDEtaDPhiPos[i]->SetXTitle("#it{E}_{cluster} (GeV)");
      
      fhTrackMatchedDEtaDPhiNeg[i]  = new TH3F
      (Form("hTrackMatchedDEtaDPhiNeg%s",cutTM[i].Data()),
       Form("#Delta #eta vs #Delta #varphi vs #it{E}_{cluster}, of cluster-negative track %s",cutTM[i].Data()),
       ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
      resBinsArray.GetSize() - 1,  resBinsArray.GetArray(),
      resBinsArray.GetSize() - 1,  resBinsArray.GetArray());
      fhTrackMatchedDEtaDPhiNeg[i]->SetZTitle("#Delta #varphi (rad)");
      fhTrackMatchedDEtaDPhiNeg[i]->SetYTitle("#Delta #eta");
      fhTrackMatchedDEtaDPhiNeg[i]->SetXTitle("#it{E}_{cluster} (GeV)");
      
      fhdEdx[i]  = new TH2F (Form("hdEdx%s",cutTM[i].Data()),Form("matched track <d#it{E}/d#it{x}> vs cluster #it{E}, %s",cutTM[i].Data()),
                             nptbins,ptmin,ptmax,ndedxbins, dedxmin, dedxmax);
      fhdEdx[i]->SetXTitle("#it{E}^{cluster} (GeV)");
      fhdEdx[i]->SetYTitle("<d#it{E}/d#it{x}>");
      
      fhEOverP[i]  = new TH2F (Form("hEOverP%s",cutTM[i].Data()),Form("matched track #it{E}/#it{p} vs cluster #it{E}, %s",cutTM[i].Data()),
                               nptbins,ptmin,ptmax,nPoverEbins,pOverEmin,pOverEmax);
      fhEOverP[i]->SetXTitle("#it{E}^{cluster} (GeV)");
      fhEOverP[i]->SetYTitle("#it{E}/#it{p}");
      
      outputContainer->Add(fhTrackMatchedDEtaDPhiPos[i]) ;
      outputContainer->Add(fhTrackMatchedDEtaDPhiNeg[i]) ;
      outputContainer->Add(fhdEdx[i]);
      outputContainer->Add(fhEOverP[i]);
      
      if ( i==0 )
      {
        fhTrackMatchedDEtaDPhiPosAfterEOverPCut  = new TH3F
        ("hTrackMatchedDEtaDPhiPosAfterEOverPCut",
         "#Delta #varphi vs #Delta #eta vs #it{E}_{cluster}  of cluster-positive track, E/p cut",
          ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
         resBinsArray.GetSize() - 1,  resBinsArray.GetArray(),
         resBinsArray.GetSize() - 1,  resBinsArray.GetArray());
        fhTrackMatchedDEtaDPhiPosAfterEOverPCut->SetYTitle("#Delta #eta");
        fhTrackMatchedDEtaDPhiPosAfterEOverPCut->SetZTitle("#Delta #varphi");
        fhTrackMatchedDEtaDPhiPosAfterEOverPCut->SetXTitle("#it{E}_{cluster} (GeV)");
        outputContainer->Add(fhTrackMatchedDEtaDPhiPosAfterEOverPCut) ;

        fhEOverPAfterResidualCut  = new TH2F 
        ("hEOverPAfterResidualCut","matched track #it{E}/#it{p} vs cluster #it{E}, residuals cut",
         nptbins,ptmin,ptmax,nPoverEbins,pOverEmin,pOverEmax);
        fhEOverPAfterResidualCut->SetXTitle("#it{E}^{cluster} (GeV)");
        fhEOverPAfterResidualCut->SetYTitle("#it{E}/#it{p}");
        outputContainer->Add(fhEOverPAfterResidualCut);
      }
      
      if ( fFillTMHistoTrackPt )
      {
        fhTrackMatchedDEtaDPhiPosTrackPt[i]  = new TH3F
        (Form("hTrackMatchedDEtaDPhiPosTrackPt%s",cutTM[i].Data()),
         Form("#Delta #eta vs #Delta #varphi vs #it{p}_{T}^{track} of cluster-positive track, %s",cutTM[i].Data()),
          ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
         resBinsArray.GetSize() - 1,  resBinsArray.GetArray(),
         resBinsArray.GetSize() - 1,  resBinsArray.GetArray());
        fhTrackMatchedDEtaDPhiPosTrackPt[i]->SetZTitle("#Delta #varphi (rad)");
        fhTrackMatchedDEtaDPhiPosTrackPt[i]->SetYTitle("#Delta #eta");
        fhTrackMatchedDEtaDPhiPosTrackPt[i]->SetXTitle("#it{p}_{T}^{track} (GeV/#it{c})");
        
        fhTrackMatchedDEtaDPhiNegTrackPt[i]  = new TH3F
        (Form("hTrackMatchedDEtaDPhiNegTrackPt%s",cutTM[i].Data()),
         Form("#Delta #eta vs #Delta #varphi vs #it{p}_{T}^{track}of cluster-negative track, %s",cutTM[i].Data()),
          ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
         resBinsArray.GetSize() - 1,  resBinsArray.GetArray(),
         resBinsArray.GetSize() - 1,  resBinsArray.GetArray());
        fhTrackMatchedDEtaDPhiNegTrackPt[i]->SetZTitle("#Delta #varphi (rad)");
        fhTrackMatchedDEtaDPhiNegTrackPt[i]->SetYTitle("#Delta #eta");
        fhTrackMatchedDEtaDPhiNegTrackPt[i]->SetXTitle("#it{p}_{T}^{track} (GeV/#it{c})");

        fhdEdxTrackPt[i]  = new TH2F (Form("hdEdxTrackPt%s",cutTM[i].Data()),Form("matched track <d#it{E}/d#it{x}> vs track #it{p}_{T}, %s",cutTM[i].Data()),
                                      nptbins,ptmin,ptmax,ndedxbins, dedxmin, dedxmax);
        fhdEdxTrackPt[i]->SetXTitle("#it{p}_{T}^{track} (GeV/#it{c})");
        fhdEdxTrackPt[i]->SetYTitle("<d#it{E}/d#it{x}>");
        
        fhEOverPTrackPt[i]  = new TH2F (Form("hEOverPTrackPt%s",cutTM[i].Data()),Form("matched track #it{E}/#it{p} vs track #it{p}_{T}, %s",cutTM[i].Data()),
                                        nptbins,ptmin,ptmax,nPoverEbins,pOverEmin,pOverEmax);
        fhEOverPTrackPt[i]->SetXTitle("#it{p}_{T}^{track} (GeV/#it{c})");
        fhEOverPTrackPt[i]->SetYTitle("#it{E}/#it{p}");
        
        outputContainer->Add(fhTrackMatchedDEtaDPhiPosTrackPt[i]) ;
        outputContainer->Add(fhTrackMatchedDEtaDPhiNegTrackPt[i]) ;
        outputContainer->Add(fhdEdxTrackPt[i]);
        outputContainer->Add(fhEOverPTrackPt[i]);
        
        if ( i==0 )
         {
           fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCut  = new TH3F
           ("hTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCut",
            "#Delta #eta vs #Delta #varphi  vs #it{p}_{T}^{track} of cluster-positive track, E/p cut",
            ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
                   resBinsArray.GetSize() - 1,  resBinsArray.GetArray(),
                   resBinsArray.GetSize() - 1,  resBinsArray.GetArray());
           fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCut->SetYTitle("#Delta #eta");
           fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCut->SetZTitle("#Delta #varphi");
           fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCut->SetXTitle("#it{p}_{T}^{track} (GeV/#it{c})");
           outputContainer->Add(fhTrackMatchedDEtaDPhiPosTrackPtAfterEOverPCut) ;

           fhEOverPTrackPtAfterResidualCut  = new TH2F 
           ("hEOverPTrackPtAfterResidualCut","matched track #it{E}/#it{p} vs cluster #it{E}, residuals cut",
            nptbins,ptmin,ptmax,nPoverEbins,pOverEmin,pOverEmax);
           fhEOverPTrackPtAfterResidualCut->SetXTitle("#it{p}_{T}^{track} (GeV/#it{c})");
           fhEOverPTrackPtAfterResidualCut->SetYTitle("#it{E}/#it{p}");
           outputContainer->Add(fhEOverPTrackPtAfterResidualCut);
         }
      }
      
      if ( GetCalorimeter()==kEMCAL &&  GetFirstSMCoveredByTRD() >=0 )
      {
        fhTrackMatchedDEtaTRD[i]  = new TH2F
        (Form("hTrackMatchedDEtaTRD%s",cutTM[i].Data()),
         Form("#Delta #eta of cluster-track vs cluster energy, SM behind TRD, %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
        fhTrackMatchedDEtaTRD[i]->SetYTitle("#Delta #eta");
        fhTrackMatchedDEtaTRD[i]->SetXTitle("#it{E}_{cluster} (GeV)");
        
        fhTrackMatchedDPhiTRD[i]  = new TH2F
        (Form("hTrackMatchedDPhiTRD%s",cutTM[i].Data()),
         Form("#Delta #varphi of cluster-track vs cluster energy, SM behing TRD, %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
        fhTrackMatchedDPhiTRD[i]->SetYTitle("#Delta #varphi (rad)");
        fhTrackMatchedDPhiTRD[i]->SetXTitle("#it{E}_{cluster} (GeV)");
        
        fhEOverPTRD[i]  = new TH2F
        (Form("hEOverPTRD%s",cutTM[i].Data()),
         Form("matched track #it{E}/#it{p} vs cluster E, behind TRD, %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nPoverEbins,pOverEmin,pOverEmax);
        fhEOverPTRD[i]->SetXTitle("#it{E} (GeV)");
        fhEOverPTRD[i]->SetYTitle("#it{E}/#it{p}");
        
        outputContainer->Add(fhTrackMatchedDEtaTRD[i]) ;
        outputContainer->Add(fhTrackMatchedDPhiTRD[i]) ;
        outputContainer->Add(fhEOverPTRD[i]);
      }
      
      if ( IsDataMC() ) 
      {
        if ( fCheckOverlaps )
        {
          fhTrackMatchedDEtaMCNoOverlap[i]  = new TH2F
          (Form("hTrackMatchedDEtaMCNoOverlap%s",cutTM[i].Data()),
           Form("#Delta #eta of cluster-track vs cluster energy, no other MC particles overlap %s",cutTM[i].Data()),
           nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
          fhTrackMatchedDEtaMCNoOverlap[i]->SetYTitle("#Delta #eta");
          fhTrackMatchedDEtaMCNoOverlap[i]->SetXTitle("#it{E}_{cluster} (GeV)");
          
          fhTrackMatchedDPhiMCNoOverlap[i]  = new TH2F
          (Form("hTrackMatchedDPhiMCNoOverlap%s",cutTM[i].Data()),
           Form("#Delta #varphi of cluster-track vs cluster energy, no other MC particles overlap %s",cutTM[i].Data()),
           nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
          fhTrackMatchedDPhiMCNoOverlap[i]->SetYTitle("#Delta #varphi (rad)");
          fhTrackMatchedDPhiMCNoOverlap[i]->SetXTitle("#it{E}_{cluster} (GeV)");
          
          outputContainer->Add(fhTrackMatchedDEtaMCNoOverlap[i]) ;
          outputContainer->Add(fhTrackMatchedDPhiMCNoOverlap[i]) ;
          fhTrackMatchedDEtaMCOverlap[i]  = new TH2F
          (Form("hTrackMatchedDEtaMCOverlap%s",cutTM[i].Data()),
           Form("#Delta #eta of cluster-track vs cluster energy, several MC particles overlap %s",cutTM[i].Data()),
           nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
          fhTrackMatchedDEtaMCOverlap[i]->SetYTitle("#Delta #eta");
          fhTrackMatchedDEtaMCOverlap[i]->SetXTitle("#it{E}_{cluster} (GeV)");
          
          fhTrackMatchedDPhiMCOverlap[i]  = new TH2F
          (Form("hTrackMatchedDPhiMCOverlap%s",cutTM[i].Data()),
           Form("#Delta #varphi of cluster-track vs cluster energy, several MC particles overlap %s",cutTM[i].Data()),
           nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
          fhTrackMatchedDPhiMCOverlap[i]->SetYTitle("#Delta #varphi (rad)");
          fhTrackMatchedDPhiMCOverlap[i]->SetXTitle("#it{E}_{cluster} (GeV)");
          
          outputContainer->Add(fhTrackMatchedDEtaMCOverlap[i]) ;
          outputContainer->Add(fhTrackMatchedDPhiMCOverlap[i]) ;
          
          fhTrackMatchedDEtaMCConversion[i]  = new TH2F
          (Form("hTrackMatchedDEtaMCConversion%s",cutTM[i].Data()),
           Form("#Delta #eta of cluster-track vs cluster energy, no other MC particles overlap appart from conversions %s",cutTM[i].Data()),
           nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
          fhTrackMatchedDEtaMCConversion[i]->SetYTitle("#Delta #eta");
          fhTrackMatchedDEtaMCConversion[i]->SetXTitle("#it{E}_{cluster} (GeV)");
          
          fhTrackMatchedDPhiMCConversion[i]  = new TH2F
          (Form("hTrackMatchedDPhiMCConversion%s",cutTM[i].Data()),
           Form("#Delta #varphi of cluster-track vs cluster energy, no other MC particles overlap appart from conversions %s",cutTM[i].Data()),
           nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
          fhTrackMatchedDPhiMCConversion[i]->SetYTitle("#Delta #varphi (rad)");
          fhTrackMatchedDPhiMCConversion[i]->SetXTitle("#it{E}_{cluster} (GeV)");
          
          outputContainer->Add(fhTrackMatchedDEtaMCConversion[i]) ;
          outputContainer->Add(fhTrackMatchedDPhiMCConversion[i]) ;
        }
        
        fhTrackMatchedMCParticleBeforeTM[i]  = new TH2F
        (Form("hTrackMatchedMCParticle%s_BeforeTM",cutTM[i].Data()),
         Form("Origin of particle vs energy %s, before TM criteria",cutTM[i].Data()),
         nptbins,ptmin,ptmax,11,0,11);
        fhTrackMatchedMCParticleBeforeTM[i]->SetXTitle("#it{E} (GeV)");
        //fhTrackMatchedMCParticleBeforeTM[i]->SetYTitle("Particle type");
        for(Int_t imcpart = 1; imcpart < 12; imcpart++)
          fhTrackMatchedMCParticleBeforeTM[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
        
        outputContainer->Add(fhTrackMatchedMCParticleBeforeTM[i]);
        
        fhTrackMatchedMCParticleTrackPtBeforeTM[i]  = new TH2F
        (Form("hTrackMatchedMCParticleTrackPt%s_BeforeTM",cutTM[i].Data()),
         Form("Origin of particle vs track #it{p}_{T} %s, before TM criteria",cutTM[i].Data()),
         nptbins,ptmin,ptmax,11,0,11);
        fhTrackMatchedMCParticleTrackPtBeforeTM[i]->SetXTitle("#it{p}_{T}^{track} (GeV/#it{c})");
        //fhTrackMatchedMCParticleTrackPtBeforeTM[i]->SetYTitle("Particle type");
        for(Int_t imcpart = 1; imcpart < 12; imcpart++)
          fhTrackMatchedMCParticleTrackPtBeforeTM[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
        
        outputContainer->Add(fhTrackMatchedMCParticleTrackPtBeforeTM[i]); 

        fhTrackMatchedMCParticle[i]  = new TH2F
        (Form("hTrackMatchedMCParticle%s",cutTM[i].Data()),
         Form("Origin of particle vs energy %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,11,0,11);
        fhTrackMatchedMCParticle[i]->SetXTitle("#it{E} (GeV)");
        //fhTrackMatchedMCParticle[i]->SetYTitle("Particle type");
        for(Int_t imcpart = 1; imcpart < 12; imcpart++)
          fhTrackMatchedMCParticle[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
       
        outputContainer->Add(fhTrackMatchedMCParticle[i]);
        
        fhTrackMatchedMCParticleTrackPt[i]  = new TH2F
        (Form("hTrackMatchedMCParticleTrackPt%s",cutTM[i].Data()),
         Form("Origin of particle vs track #it{p}_{T} %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,11,0,11);
        fhTrackMatchedMCParticleTrackPt[i]->SetXTitle("#it{p}_{T}^{track} (GeV/#it{c})");
        //fhTrackMatchedMCParticleTrackPt[i]->SetYTitle("Particle type");
        for(Int_t imcpart = 1; imcpart < 12; imcpart++)
          fhTrackMatchedMCParticleTrackPt[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
        
        outputContainer->Add(fhTrackMatchedMCParticleTrackPt[i]); 
        
        fhTrackMatchedMCParticleConverted[i]  = new TH2F
        (Form("hTrackMatchedMCParticle%s_Converted",cutTM[i].Data()),
         Form("Origin of particle vs energy %s, suffer conversion",cutTM[i].Data()),
         nptbins,ptmin,ptmax,11,0,11);
        fhTrackMatchedMCParticleConverted[i]->SetXTitle("#it{E} (GeV)");
        //fhTrackMatchedMCParticle[i]->SetYTitle("Particle type");
        for(Int_t imcpart = 1; imcpart < 12; imcpart++)
          fhTrackMatchedMCParticleConverted[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
       
        outputContainer->Add(fhTrackMatchedMCParticleConverted[i]);
  
        fhTrackMatchedMCParticleVsEOverP[i]  = new TH3F
        (Form("hTrackMatchedMCParticle%s_EOverP",cutTM[i].Data()),
         Form("Origin of particle vs energy %s vs #it{E}/#it{p}",cutTM[i].Data()),
         nptbins,ptmin,ptmax,11,0,11,100,0,20);
        fhTrackMatchedMCParticleVsEOverP[i]->SetXTitle("#it{E} (GeV)");
        fhTrackMatchedMCParticleVsEOverP[i]->SetZTitle("#it{E}/#it{p}");
        //fhTrackMatchedMCParticle[i]->SetYTitle("Particle type");
        for(Int_t imcpart = 1; imcpart < 12; imcpart++)
          fhTrackMatchedMCParticleVsEOverP[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
        
        outputContainer->Add(fhTrackMatchedMCParticleVsEOverP[i]);
        
        if ( fCheckOverlaps )
        {
          fhTrackMatchedMCParticleVsNOverlaps[i]  = new TH3F
          (Form("hTrackMatchedMCParticle%s_NOverlaps",cutTM[i].Data()),
           Form("Origin of particle vs energy %s vs number of particle overlaps",cutTM[i].Data()),
           nptbins,ptmin,ptmax,11,0,11,10,0,10);
          fhTrackMatchedMCParticleVsNOverlaps[i]->SetXTitle("#it{E} (GeV)");
          fhTrackMatchedMCParticleVsNOverlaps[i]->SetZTitle("#it{n}_{overlaps}");
          //fhTrackMatchedMCParticle[i]->SetYTitle("Particle type");
          for(Int_t imcpart = 1; imcpart < 12; imcpart++)
            fhTrackMatchedMCParticleVsNOverlaps[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
          
          outputContainer->Add(fhTrackMatchedMCParticleVsNOverlaps[i]);
        }
        
        fhTrackMatchedMCParticleVsErecEgen[i]  = new TH3F
        (Form("hTrackMatchedMCParticle%s_ErecEgenRat",cutTM[i].Data()),
         Form("Origin of particle vs energy %s vs #it{E}_{rec}/#it{E}_{gen}",cutTM[i].Data()),
         nptbins,ptmin,ptmax,11,0,11,100,0,2);
        fhTrackMatchedMCParticleVsErecEgen[i]->SetXTitle("#it{E}_{rec} (GeV)");
        fhTrackMatchedMCParticleVsErecEgen[i]->SetZTitle("#it{E}_{rec}/#it{E}_{gen}");
        //fhTrackMatchedMCParticle[i]->SetYTitle("Particle type");
        for(Int_t imcpart = 1; imcpart < 12; imcpart++)
          fhTrackMatchedMCParticleVsErecEgen[i]->GetYaxis()->SetBinLabel(imcpart,mcPartLabels[imcpart-1]);
        
        outputContainer->Add(fhTrackMatchedMCParticleVsErecEgen[i]);
        
      }
    }
  }
  
  if(IsPileUpAnalysisOn())
  {
    TString pileUpName[] = {"SPD","EMCAL","SPDOrEMCAL","SPDAndEMCAL","SPDAndNotEMCAL","EMCALAndNotSPD","NotSPDAndNotEMCAL"} ;
    
    for(Int_t i = 0 ; i < 7 ; i++)
    {
      fhPtPhotonPileUp[i]  = new TH1F(Form("hPtPhotonPileUp%s",pileUpName[i].Data()),
                                      Form("Selected photon #it{p}_{T} distribution, %s Pile-Up event",pileUpName[i].Data()), nptbins,ptmin,ptmax);
      fhPtPhotonPileUp[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhPtPhotonPileUp[i]);
      
      fhClusterTimeDiffPhotonPileUp[i]  = new TH2F(Form("hClusterTimeDiffPhotonPileUp%s",pileUpName[i].Data()),
                                             Form("Photon cluster E vs #it{t}_{max}-#it{t}_{cell} in cluster, %s Pile-Up event",pileUpName[i].Data()),
                                             nptbins,ptmin,ptmax,400,-200,200);
      fhClusterTimeDiffPhotonPileUp[i]->SetXTitle("#it{E} (GeV)");
      fhClusterTimeDiffPhotonPileUp[i]->SetYTitle("#it{t}_{max}-#it{t}_{cell} (ns)");
      outputContainer->Add(fhClusterTimeDiffPhotonPileUp[i]);
    }
    
    fhTimePtPhotonNoCut  = new TH2F ("hTimePtPhoton_NoCut","time of photon cluster vs pT of clusters, no cut", nptbins,ptmin,ptmax, ntimebins,timemin,timemax);
    fhTimePtPhotonNoCut->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    fhTimePtPhotonNoCut->SetYTitle("#it{time} (ns)");
    outputContainer->Add(fhTimePtPhotonNoCut);
    
    fhTimePtPhotonSPD  = new TH2F ("hTimePtPhoton_SPD","time of  photon cluster vs pT of clusters, SPD cut", nptbins,ptmin,ptmax, ntimebins,timemin,timemax);
    fhTimePtPhotonSPD->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    fhTimePtPhotonSPD->SetYTitle("#it{time} (ns)");
    outputContainer->Add(fhTimePtPhotonSPD);
    
    fhTimeNPileUpVertSPD  = new TH2F ("hTime_NPileUpVertSPD","time of cluster vs N pile-up SPD vertex", ntimebins,timemin,timemax,20,0,20);
    fhTimeNPileUpVertSPD->SetYTitle("# vertex ");
    fhTimeNPileUpVertSPD->SetXTitle("#it{time} (ns)");
    outputContainer->Add(fhTimeNPileUpVertSPD);
    
    fhTimeNPileUpVertTrack  = new TH2F ("hTime_NPileUpVertTracks","time of cluster vs N pile-up Tracks vertex", ntimebins,timemin,timemax, 20,0,20 );
    fhTimeNPileUpVertTrack->SetYTitle("# vertex ");
    fhTimeNPileUpVertTrack->SetXTitle("#it{time} (ns)");
    outputContainer->Add(fhTimeNPileUpVertTrack);
    
    fhPtPhotonNPileUpSPDVtx  = new TH2F ("hPtPhoton_NPileUpVertSPD","pT of cluster vs N pile-up SPD vertex",
                                         nptbins,ptmin,ptmax,20,0,20);
    fhPtPhotonNPileUpSPDVtx->SetYTitle("# vertex ");
    fhPtPhotonNPileUpSPDVtx->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpSPDVtx);
	  
    fhPtPhotonNPileUpTrkVtx  = new TH2F ("hPtPhoton_NPileUpVertTracks","pT of cluster vs N pile-up Tracks vertex",
                                         nptbins,ptmin,ptmax, 20,0,20 );
    fhPtPhotonNPileUpTrkVtx->SetYTitle("# vertex ");
    fhPtPhotonNPileUpTrkVtx->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpTrkVtx);
	  
    fhPtPhotonNPileUpSPDVtxTimeCut  = new TH2F ("hPtPhoton_NPileUpVertSPD_TimeCut","pT of cluster vs N pile-up SPD vertex, |tof| < 25 ns",
                                                nptbins,ptmin,ptmax,20,0,20);
    fhPtPhotonNPileUpSPDVtxTimeCut->SetYTitle("# vertex ");
    fhPtPhotonNPileUpSPDVtxTimeCut->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpSPDVtxTimeCut);
	  
    fhPtPhotonNPileUpTrkVtxTimeCut  = new TH2F ("hPtPhoton_NPileUpVertTracks_TimeCut","pT of cluster vs N pile-up Tracks vertex, |tof| < 25 ns",
                                                nptbins,ptmin,ptmax, 20,0,20 );
    fhPtPhotonNPileUpTrkVtxTimeCut->SetYTitle("# vertex ");
    fhPtPhotonNPileUpTrkVtxTimeCut->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpTrkVtxTimeCut);
	  
    fhPtPhotonNPileUpSPDVtxTimeCut2  = new TH2F ("hPtPhoton_NPileUpVertSPD_TimeCut2","pT of cluster vs N pile-up SPD vertex, -25 < tof < 75 ns",
                                                 nptbins,ptmin,ptmax,20,0,20);
    fhPtPhotonNPileUpSPDVtxTimeCut2->SetYTitle("# vertex ");
    fhPtPhotonNPileUpSPDVtxTimeCut2->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpSPDVtxTimeCut2);
	  
    fhPtPhotonNPileUpTrkVtxTimeCut2  = new TH2F ("hPtPhoton_NPileUpVertTracks_TimeCut2","pT of cluster vs N pile-up Tracks vertex,  -25 < tof < 75 ns",
                                                 nptbins,ptmin,ptmax, 20,0,20 );
    fhPtPhotonNPileUpTrkVtxTimeCut2->SetYTitle("# vertex ");
    fhPtPhotonNPileUpTrkVtxTimeCut2->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpTrkVtxTimeCut2);
    
  }

  if(GetCalorimeter() == kEMCAL && fFillEMCALRegionSSHistograms && fFillSSHistograms)
  {
    for(Int_t ieta = 0; ieta < 4; ieta++) 
    {  
      for(Int_t iphi = 0; iphi < 3; iphi++) 
      {
//        fhLam0EMCALRegion[ieta][iphi] = 
//        new TH2F(Form("hLam0_eta%d_phi%d",ieta,iphi),
//                 Form("cluster from converted photon, #it{p}_{T} vs #sigma^{2}_{long}, region eta %d, phi %d",ieta,iphi),
//                 nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
//        fhLam0EMCALRegion[ieta][iphi]->SetYTitle("#sigma^{2}_{long}");
//        fhLam0EMCALRegion[ieta][iphi]->SetXTitle("#it{p}_{T} (GeV)");
//        outputContainer->Add(fhLam0EMCALRegion[ieta][iphi]) ;
//        
//        if(GetFirstSMCoveredByTRD() >= 0)
//        {
//          fhLam0EMCALRegionTRD[ieta][iphi] = 
//          new TH2F(Form("hLam0TRD_eta%d_phi%d",ieta,iphi),
//                   Form("cluster from converted photon, #it{p}_{T} vs #sigma^{2}_{long}, region eta %d, phi %d, SM covered by TRD",ieta,iphi),
//                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
//          fhLam0EMCALRegionTRD[ieta][iphi]->SetYTitle("#sigma^{2}_{long}");
//          fhLam0EMCALRegionTRD[ieta][iphi]->SetXTitle("#it{p}_{T} (GeV)");
//          outputContainer->Add(fhLam0EMCALRegionTRD[ieta][iphi]) ;
//        } // TRD
        
        for(Int_t ism = 0; ism < fNModules; ism++) 
        {
          if(ism < fFirstModule || ism > fLastModule) continue;

          fhLam0EMCALRegionPerSM[ieta][iphi][ism] = 
          new TH2F(Form("hLam0_eta%d_phi%d_sm%d",ieta,iphi,ism),
                   Form("cluster from converted photon, #it{p}_{T} vs #sigma^{2}_{long}, sm %d, region eta %d, phi %d",ism,ieta,iphi),
                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhLam0EMCALRegionPerSM[ieta][iphi][ism]->SetYTitle("#sigma^{2}_{long}");
          fhLam0EMCALRegionPerSM[ieta][iphi][ism]->SetXTitle("#it{p}_{T} (GeV)");
          outputContainer->Add(fhLam0EMCALRegionPerSM[ieta][iphi][ism]) ;
          
          fhLam1EMCALRegionPerSM[ieta][iphi][ism] = 
          new TH2F(Form("hLam1_eta%d_phi%d_sm%d",ieta,iphi,ism),
                   Form("cluster from converted photon, #it{p}_{T} vs #sigma^{2}_{short}, sm %d, region eta %d, phi %d",ism,ieta,iphi),
                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhLam1EMCALRegionPerSM[ieta][iphi][ism]->SetYTitle("#sigma^{2}_{short}");
          fhLam1EMCALRegionPerSM[ieta][iphi][ism]->SetXTitle("#it{p}_{T} (GeV)");
          outputContainer->Add(fhLam1EMCALRegionPerSM[ieta][iphi][ism]) ;
        }
      } // iphi 
    } // ieta
    
    Float_t ptLimit[] = {2,3,4,5,6,8,10,12};
    TString l0bin  [] = {"0.23<#sigma^{2}_{long}<0.26","0.3<#sigma^{2}_{long}<0.4"};
    
    for(Int_t il0 = 0; il0 < 2; il0++)
    {
      for(Int_t ipt = 0; ipt < 7; ipt++)
      {
        fhEtaPhiLam0BinPtBin[il0][ipt]  = new TH2F
        (Form("hEtaPhiLam0Bin%d_PtBin%d",il0,ipt),
         Form("#eta vs #varphi in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         netabins,etamin,etamax,nphibins,phimin,phimax);
        fhEtaPhiLam0BinPtBin[il0][ipt]->SetYTitle("#varphi (rad)");
        fhEtaPhiLam0BinPtBin[il0][ipt]->SetXTitle("#eta");
        outputContainer->Add(fhEtaPhiLam0BinPtBin[il0][ipt]) ;
        
        fhColRowLam0BinPtBin[il0][ipt]  = new TH2F
        (Form("hColRowLam0Bin%d_PtBin%d",il0,ipt),
         Form("row vs column in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s, w > 0",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         ncolcell,colcellmin,colcellmax,nrowcell,rowcellmin,rowcellmax);
        fhColRowLam0BinPtBin[il0][ipt]->SetYTitle("row");
        fhColRowLam0BinPtBin[il0][ipt]->SetXTitle("column");
        outputContainer->Add(fhColRowLam0BinPtBin[il0][ipt]) ;   
        
        fhColRowLam0BinPtBinWeighted[il0][ipt]  = new TH2F
        (Form("hColRowLam0Bin%d_PtBin%dWeighted",il0,ipt),
         Form("cluster cell row vs column weighted in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         ncolcell,colcellmin,colcellmax,nrowcell,rowcellmin,rowcellmax);
        fhColRowLam0BinPtBinWeighted[il0][ipt]->SetYTitle("row");
        fhColRowLam0BinPtBinWeighted[il0][ipt]->SetXTitle("column");
        outputContainer->Add(fhColRowLam0BinPtBinWeighted[il0][ipt]) ;
        
        fhColRowLam0BinPtBinLargeTime[il0][ipt]  = new TH2F
        (Form("hColRowLam0Bin%d_PtBin%d_LargeTimeInClusterCell",il0,ipt),
         Form("row vs column in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s, |t| > 50 ns, w > 0",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         ncolcell,colcellmin,colcellmax,nrowcell,rowcellmin,rowcellmax);
        fhColRowLam0BinPtBinLargeTime[il0][ipt]->SetYTitle("row");
        fhColRowLam0BinPtBinLargeTime[il0][ipt]->SetXTitle("column");
        outputContainer->Add(fhColRowLam0BinPtBinLargeTime[il0][ipt]) ;  
        
//        fhEtaPhiLam0BinPtBinSMShared[il0][ipt]  = new TH2F
//        (Form("hEtaPhiLam0Bin%d_PtBin%d_SMShared",il0,ipt),
//         Form("#eta vs #varphi in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s",
//              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
//         netabins,etamin,etamax,nphibins,phimin,phimax);
//        fhEtaPhiLam0BinPtBinSMShared[il0][ipt]->SetYTitle("#varphi (rad)");
//        fhEtaPhiLam0BinPtBinSMShared[il0][ipt]->SetXTitle("#eta");
//        outputContainer->Add(fhEtaPhiLam0BinPtBinSMShared[il0][ipt]) ;
//        
//        fhColRowLam0BinPtBinSMShared[il0][ipt]  = new TH2F
//        (Form("hColRowLam0Bin%d_PtBin%d_SMShared",il0,ipt),
//         Form("row vs column in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s, w > 0",
//              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
//         ncolcell,colcellmin,colcellmax,nrowcell,rowcellmin,rowcellmax);
//        fhColRowLam0BinPtBinSMShared[il0][ipt]->SetYTitle("row");
//        fhColRowLam0BinPtBinSMShared[il0][ipt]->SetXTitle("column");
//        outputContainer->Add(fhColRowLam0BinPtBinSMShared[il0][ipt]) ;  
        
        fhEtaPhiLargeTimeInClusterCell[il0][ipt]  = new TH2F
        (Form("hEtaPhiLam0Bin%d_PtBin%d_LargeTimeInClusterCell",il0,ipt),
         Form("#eta vs #varphi in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, w > 0, t > 50 ns, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         netabins,etamin,etamax,nphibins,phimin,phimax);
        fhEtaPhiLargeTimeInClusterCell[il0][ipt]->SetYTitle("#varphi (rad)");
        fhEtaPhiLargeTimeInClusterCell[il0][ipt]->SetXTitle("#eta");
        outputContainer->Add(fhEtaPhiLargeTimeInClusterCell[il0][ipt]) ;
        
        fhCellClusterIndexEAndTime[il0][ipt]  = new TH2F
        (Form("hCellClusterIndexEAndTimeLam0Bin%d_PtBin%d",il0,ipt),
         Form("#it{t}_{cell} vs cell index (E sorted) in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, w > 0, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         ntimebins,timemin,timemax,30,0,30);
        fhCellClusterIndexEAndTime[il0][ipt] ->SetYTitle("Index (sorted #it{E})");
        fhCellClusterIndexEAndTime[il0][ipt] ->SetXTitle("#it{time}_{cell} (ns)");
        outputContainer->Add(fhCellClusterIndexEAndTime[il0][ipt] ) ;    

        fhCellClusterEAndTime[il0][ipt]  = new TH2F
        (Form("hCellClusterEAndTimeLam0Bin%d_PtBin%d",il0,ipt),
         Form("#it{E}_{cell} vs #it{t}_{cell}  in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, w > 0, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         ntimebins,timemin,timemax,100,0,5);
        fhCellClusterEAndTime[il0][ipt] ->SetYTitle("#it{E}_{cell} (GeV)");
        fhCellClusterEAndTime[il0][ipt] ->SetXTitle("#it{time}_{cell} (ns)");
        outputContainer->Add(fhCellClusterEAndTime[il0][ipt] ) ;    

        fhCellClusterEFracAndTime[il0][ipt]  = new TH2F
        (Form("hCellClusterEFracAndTimeLam0Bin%d_PtBin%d",il0,ipt),
         Form("#it{E}_{cell} vs #it{t}_{cell}  in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, w > 0, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         ntimebins,timemin,timemax,100,0,1);
        fhCellClusterEFracAndTime[il0][ipt] ->SetYTitle("#it{E}_{cell} (GeV)");
        fhCellClusterEFracAndTime[il0][ipt] ->SetXTitle("#it{time}_{cell} (ns)");
        outputContainer->Add(fhCellClusterEFracAndTime[il0][ipt] ) ;            
      } // pt bin
      
      for(Int_t ism = 0; ism < fNModules; ism++)
      {
        if(ism < fFirstModule || ism > fLastModule) continue;

        fhLam1Lam0BinPerSM[il0][ism] = new TH2F
        (Form("hLam1Lam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs #sigma^{2}_{short} in sm %d, %s",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,40,0,0.4);
        fhLam1Lam0BinPerSM[il0][ism]->SetYTitle("#sigma^{2}_{short}");
        fhLam1Lam0BinPerSM[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhLam1Lam0BinPerSM[il0][ism]) ;   
        
        fhTimeLam0BinPerSM[il0][ism] = new TH2F
        (Form("hTimeLam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs cluster cell time in sm %d, %s, w > 0",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,ntimebins,timemin,timemax);
        fhTimeLam0BinPerSM[il0][ism]->SetYTitle("cell time (ns)");
        fhTimeLam0BinPerSM[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhTimeLam0BinPerSM[il0][ism]) ;   
        
        fhTimeLam0BinPerSMWeighted[il0][ism] = new TH2F
        (Form("hTimeLam0Bin%d_sm%d_Weighted",il0,ism),
         Form("#it{p}_{T} vs cluster cell time weighted in sm %d, %s",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,ntimebins,timemin,timemax);
        fhTimeLam0BinPerSMWeighted[il0][ism]->SetYTitle("cell time (ns)");
        fhTimeLam0BinPerSMWeighted[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhTimeLam0BinPerSMWeighted[il0][ism]) ;   
        
        fhDTimeLam0BinPerSM[il0][ism] = new TH2F
        (Form("hDTimeLam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs t_{cluster}-t_{cell} in sm %d, %s, w > 0",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,ntimebins,timemin,timemax);
        fhDTimeLam0BinPerSM[il0][ism]->SetYTitle("t_{cluster}-t_{cell} (ns)");
        fhDTimeLam0BinPerSM[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhDTimeLam0BinPerSM[il0][ism]) ;   
        
        fhDTimeLam0BinPerSMWeighted[il0][ism] = new TH2F
        (Form("hDTimeLam0Bin%d_sm%d_Weighted",il0,ism),
         Form("#it{p}_{T} vs t_{cluster}-t_{cell} weighted in sm %d, %s",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,ntimebins,timemin,timemax);
        fhDTimeLam0BinPerSMWeighted[il0][ism]->SetYTitle("t_{cluster}-t_{cell} (ns)");
        fhDTimeLam0BinPerSMWeighted[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhDTimeLam0BinPerSMWeighted[il0][ism]) ;   
        
        fhCellClusterEFracLam0BinPerSM[il0][ism] = new TH2F
        (Form("hCellClusterEFracLam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs cell E / cluster E in sm %d, %s, w > 0",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,100,0,1);
        fhCellClusterEFracLam0BinPerSM[il0][ism]->SetYTitle("cell E / cluster E ");
        fhCellClusterEFracLam0BinPerSM[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterEFracLam0BinPerSM[il0][ism]) ;         
        
//        fhCellClusterEFracLam0BinPerSMWeighted[il0][ism] = new TH2F
//        (Form("hCellClusterEFracLam0Bin%d_sm%d_Weighted",il0,ism),
//         Form("#it{p}_{T} vs cell E / cluster E weighted in sm %d, %s",ism,l0bin[il0].Data()),
//         nptbins,ptmin,ptmax,100,0,1);
//        fhCellClusterEFracLam0BinPerSMWeighted[il0][ism]->SetYTitle("cell E / cluster E ");
//        fhCellClusterEFracLam0BinPerSMWeighted[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//        outputContainer->Add(fhCellClusterEFracLam0BinPerSMWeighted[il0][ism]) ;         
        
        fhCellClusterEFracLam0BinPerSMLargeTime[il0][ism] = new TH2F
        (Form("hCellClusterEFracLam0Bin%d_sm%d_LargeTimeInClusterCell",il0,ism),
         Form("#it{p}_{T} vs cell E / cluster E in sm %d, %s, w > 0, |t_{cell}| > 50 ns",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,100,0,1);
        fhCellClusterEFracLam0BinPerSMLargeTime[il0][ism]->SetYTitle("cell E / cluster E ");
        fhCellClusterEFracLam0BinPerSMLargeTime[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterEFracLam0BinPerSMLargeTime[il0][ism]) ;         
        
        fhCellClusterEFracLam0BinPerSMLargeTimeTotal[il0][ism] = new TH2F
        (Form("hCellClusterEFracLam0Bin%d_sm%d_LargeTimeInClusterCell_Total",il0,ism),
         Form("#it{p}_{T} vs cell E / cluster E in sm %d, %s, w > 0, all |t_{cell}| > 50 ns",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,100,0,1);
        fhCellClusterEFracLam0BinPerSMLargeTimeTotal[il0][ism]->SetYTitle("cell E / cluster E ");
        fhCellClusterEFracLam0BinPerSMLargeTimeTotal[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterEFracLam0BinPerSMLargeTimeTotal[il0][ism]) ;
        
        fhCellClusterELam0BinPerSM[il0][ism] = new TH2F
        (Form("hCellClusterELam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs cell E in sm %d, %s, w > 0",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,500,0,10);
        fhCellClusterELam0BinPerSM[il0][ism]->SetYTitle("cell E (GeV)");
        fhCellClusterELam0BinPerSM[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterELam0BinPerSM[il0][ism]) ;         
        
        fhCellClusterELam0BinPerSMWeighted[il0][ism] = new TH2F
        (Form("hCellClusterELam0Bin%d_sm%d_Weighted",il0,ism),
         Form("#it{p}_{T} vs cell E  weighted in sm %d, %s",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,500,0,10);
        fhCellClusterELam0BinPerSMWeighted[il0][ism]->SetYTitle("cell E (GeV)");
        fhCellClusterELam0BinPerSMWeighted[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterELam0BinPerSMWeighted[il0][ism]) ;   
        
        fhCellClusterELam0BinPerSMLargeTime[il0][ism] = new TH2F
        (Form("hCellClusterELam0Bin%d_sm%d_LargeTimeInClusterCell",il0,ism),
         Form("#it{p}_{T} vs cell E in sm %d, %s, w > 0, |t_{cell}| > 50 ns",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,500,0,10);
        fhCellClusterELam0BinPerSMLargeTime[il0][ism]->SetYTitle("cell E (GeV)");
        fhCellClusterELam0BinPerSMLargeTime[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterELam0BinPerSMLargeTime[il0][ism]) ;      

        fhCellClusterIndexELam0BinPerSMLargeTime[il0][ism] = new TH2F
        (Form("hCellClusterIndexELam0Bin%d_sm%d_LargeTimeInClusterCell",il0,ism),
         Form("#it{p}_{T} vs cell index (E sorted) in sm %d, %s, w > 0, |t_{cell}| > 50 ns",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,30,0,30);
        fhCellClusterIndexELam0BinPerSMLargeTime[il0][ism]->SetYTitle("Index");
        fhCellClusterIndexELam0BinPerSMLargeTime[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterIndexELam0BinPerSMLargeTime[il0][ism]) ;      
        
        fhNCellsWithLargeTimeInCluster[il0][ism] = new TH2F
        (Form("hNCellsWithLargeTimeInClusterLam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs number of cells in sm %d, %s, w > 0, |t_{cell}| > 50 ns",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,30,0,30);
        fhNCellsWithLargeTimeInCluster[il0][ism]->SetYTitle("#it{n}_{cells}");
        fhNCellsWithLargeTimeInCluster[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhNCellsWithLargeTimeInCluster[il0][ism]) ;      
        
//        if(ism < 12)
//        {
//          fhTimeLam0BinPerSMShared[il0][ism] = new TH2F
//          (Form("hTimeLam0Bin%d_sm%d_SMShared",il0,ism),
//           Form("#it{p}_{T} vs cluster cell time in sm %d, %s, w > 0",ism,l0bin[il0].Data()),
//           nptbins,ptmin,ptmax,ntimebins,timemin,timemax);
//          fhTimeLam0BinPerSMShared[il0][ism]->SetYTitle("cell time (ns)");
//          fhTimeLam0BinPerSMShared[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//          outputContainer->Add(fhTimeLam0BinPerSMShared[il0][ism]) ;             
//        } // Run 1 SM
      } // sm
    } // l0 bin 
    
    for(Int_t ism = 0; ism < fNModules; ism++)
    {
      if(ism < fFirstModule || ism > fLastModule) continue;
      
      fhLam0PerSMLargeTimeInClusterCell[ism] = new TH2F
      (Form("hLam0_sm%d_LargeTimeInClusterCell",ism),
       Form("#it{p}_{T} vs #sigma^{2}_{long} in sm %d,|t_{secondary cell}| > 50 ns",ism),
       nptbins,ptmin,ptmax,40,0,0.4);
      fhLam0PerSMLargeTimeInClusterCell[ism]->SetYTitle("#sigma^{2}_{long}");
      fhLam0PerSMLargeTimeInClusterCell[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PerSMLargeTimeInClusterCell[ism]) ;             
      
      fhLam1PerSMLargeTimeInClusterCell[ism] = new TH2F
      (Form("hLam1_sm%d_LargeTimeInClusterCell",ism),
       Form("#it{p}_{T} vs #sigma^{2}_{short} in sm %d, |t_{secondary cell}| > 50 ns",ism),
       nptbins,ptmin,ptmax,40,0,0.4);
      fhLam1PerSMLargeTimeInClusterCell[ism]->SetYTitle("#sigma^{2}_{short}");
      fhLam1PerSMLargeTimeInClusterCell[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam1PerSMLargeTimeInClusterCell[ism]) ;   
      
//      fhLam0PerSMSPDPileUp[ism] = new TH2F
//      (Form("hLam0_sm%d_SPDPileUp",ism),
//       Form("#it{p}_{T} vs #sigma^{2}_{long} in sm %d, Pile-up event SPD",ism),
//       nptbins,ptmin,ptmax,40,0,0.4);
//      fhLam0PerSMSPDPileUp[ism]->SetYTitle("#sigma^{2}_{long}");
//      fhLam0PerSMSPDPileUp[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//      outputContainer->Add(fhLam0PerSMSPDPileUp[ism]) ;             
//      
//      fhLam1PerSMSPDPileUp[ism] = new TH2F
//      (Form("hLam1_sm%d_SPDPileUp",ism),
//       Form("#it{p}_{T} vs #sigma^{2}_{short} in sm %d, Pile-up event SPD",ism),
//       nptbins,ptmin,ptmax,40,0,0.4);
//      fhLam1PerSMSPDPileUp[ism]->SetYTitle("#sigma^{2}_{short}");
//      fhLam1PerSMSPDPileUp[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//      outputContainer->Add(fhLam1PerSMSPDPileUp[ism]) ;   
      
//      if(ism < 12)
//      {
//        fhLam0PerSMShared[ism] = new TH2F
//        (Form("hLam0_sm%d_SMShared",ism),
//         Form("#it{p}_{T} vs #sigma^{2}_{long} in sm %d, SM shared",ism),
//         nptbins,ptmin,ptmax,40,0,0.4);
//        fhLam0PerSMShared[ism]->SetYTitle("#sigma^{2}_{long}");
//        fhLam0PerSMShared[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//        outputContainer->Add(fhLam0PerSMShared[ism]) ;             
//        
//        fhLam1PerSMShared[ism] = new TH2F
//        (Form("hLam1_sm%d_SMShared",ism),
//         Form("#it{p}_{T} vs #sigma^{2}_{short} in sm %d, SM shared",ism),
//         nptbins,ptmin,ptmax,40,0,0.4);
//        fhLam1PerSMShared[ism]->SetYTitle("#sigma^{2}_{short}");
//        fhLam1PerSMShared[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//        outputContainer->Add(fhLam1PerSMShared[ism]) ;   
//      } // run1
    } // sm
    
    for(Int_t ilarge = 0; ilarge < 5; ilarge++)
    {
      fhLam0PerNLargeTimeInClusterCell[ilarge] = new TH2F
      (Form("hLam0_NLargeTimeInClusterCell%d",ilarge),
       Form("#it{p}_{T} vs #sigma^{2}_{long} in sm %d,|t_{secondary cell}| > 50 ns",ilarge),
       nptbins,ptmin,ptmax,40,0,0.4);
      fhLam0PerNLargeTimeInClusterCell[ilarge]->SetYTitle("#sigma^{2}_{long}");
      fhLam0PerNLargeTimeInClusterCell[ilarge]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PerNLargeTimeInClusterCell[ilarge]) ;             
      
      fhLam1PerNLargeTimeInClusterCell[ilarge] = new TH2F
      (Form("hLam1_NLargeTimeInClusterCell%d",ilarge),
       Form("#it{p}_{T} vs #sigma^{2}_{short} in sm %d, |t_{secondary cell}| > 50 ns",ilarge),
       nptbins,ptmin,ptmax,40,0,0.4);
      fhLam1PerNLargeTimeInClusterCell[ilarge]->SetYTitle("#sigma^{2}_{short}");
      fhLam1PerNLargeTimeInClusterCell[ilarge]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam1PerNLargeTimeInClusterCell[ilarge]) ;   
    }
    
  } // regions in EMCal

  
  if ( IsDataMC() )
  {
    TString ptype[] = { "#gamma"         , "#gamma_{#pi decay}"    , "#gamma_{#eta decay}", "#gamma_{other decay}",
                        "#pi^{0}"        , "#eta"                  , "e^{#pm}"            , "#gamma->e^{#pm}"     ,
                        "hadron?"        , "Anti-N"                , "Anti-P"             , 
                        "Neutron"        , "Proton"                , "#pi^{#pm}"          ,
                        "#gamma_{prompt}", "#gamma_{fragmentation}", "#gamma_{ISR}"       , "String"               } ;
    
    TString pname[] = { "Photon"      , "PhotonPi0Decay"     , "PhotonEtaDecay", "PhotonOtherDecay",
                        "Pi0"         , "Eta"                , "Electron"      , "Conversion"      ,
                        "Hadron"      , "AntiNeutron"        , "AntiProton"    , 
                        "Neutron"     , "Proton"             , "ChPion"        ,
                        "PhotonPrompt", "PhotonFragmentation", "PhotonISR"     , "String"           } ;
    
    for(Int_t i = 0; i < fNOriginHistograms; i++)
    {
      if ( !fFillOnlyPtHisto )
      {
        fhMCE[i]  = new TH1F(Form("hE_MC%s",pname[i].Data()),
                             Form("cluster from %s : E ",ptype[i].Data()),
                             nptbins,ptmin,ptmax);
        fhMCE[i]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMCE[i]) ;
        fhMCDeltaE[i]  = new TH2F (Form("hDeltaE_MC%s",pname[i].Data()),
                                   Form("MC - Reco E from %s",pname[i].Data()),
                                   nptbins,ptmin,ptmax, 200,-50,50);
        fhMCDeltaE[i]->SetYTitle("#Delta #it{E} (GeV)");
        fhMCDeltaE[i]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMCDeltaE[i]);
        
        fhMC2E[i]  = new TH2F (Form("h2E_MC%s",pname[i].Data()),
                               Form("E distribution, reconstructed vs generated from %s",pname[i].Data()),
                               nptbins,ptmin,ptmax,nptbins,ptmin,ptmax);
        fhMC2E[i]->SetXTitle("#it{E}_{rec} (GeV)");
        fhMC2E[i]->SetYTitle("#it{E}_{gen} (GeV)");
        outputContainer->Add(fhMC2E[i]);
      }
     
      fhMCPt[i]  = new TH1F(Form("hPt_MC%s",pname[i].Data()),
                            Form("cluster from %s : #it{p}_{T} ",ptype[i].Data()),
                            nptbins,ptmin,ptmax);
      fhMCPt[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhMCPt[i]) ;
      
      fhMCDeltaPt[i]  = new TH2F (Form("hDeltaPt_MC%s",pname[i].Data()),
                                  Form("MC - Reco #it{p}_{T} from %s",pname[i].Data()),
                                  nptbins,ptmin,ptmax, 200,-50,50);
      fhMCDeltaPt[i]->SetXTitle("p_{T,rec} (GeV/#it{c})");
      fhMCDeltaPt[i]->SetYTitle("#Delta #it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhMCDeltaPt[i]);
      
      fhMC2Pt[i]  = new TH2F (Form("h2Pt_MC%s",pname[i].Data()),
                              Form("p_T distribution, reconstructed vs generated from %s",pname[i].Data()),
                              nptbins,ptmin,ptmax,nptbins,ptmin,ptmax);
      fhMC2Pt[i]->SetXTitle("p_{T,rec} (GeV/#it{c})");
      fhMC2Pt[i]->SetYTitle("p_{T,gen} (GeV/#it{c})");
      outputContainer->Add(fhMC2Pt[i]);
      
      fhMCEta[i]  = new TH2F(Form("hEta_MC%s",pname[i].Data()),
                             Form("cluster from %s : #eta ",ptype[i].Data()),
                             nptbins,ptmin,ptmax,netabins,etamin,etamax);
      fhMCEta[i]->SetYTitle("#eta");
      fhMCEta[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhMCEta[i]) ;
      
      fhMCPhi[i]  = new TH2F(Form("hPhi_MC%s",pname[i].Data()),
                             Form("cluster from %s : #varphi ",ptype[i].Data()),
                             nptbins,ptmin,ptmax,nphibins,phimin,phimax);
      fhMCPhi[i]->SetYTitle("#varphi (rad)");
      fhMCPhi[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhMCPhi[i]) ;
    }
    
    TString pptype[] = { "#gamma"             , "#gamma_{#pi decay}"    ,
                         "#gamma_{#eta decay}", "#gamma_{other decay}"  ,
                         "#gamma_{prompt}"    , "#gamma_{fragmentation}", "#gamma_{ISR}" } ;
    
    TString ppname[] = { "Photon"        , "PhotonPi0Decay"     ,
                         "PhotonEtaDecay", "PhotonOtherDecay"   ,
                         "PhotonPrompt"  , "PhotonFragmentation", "PhotonISR" } ;
    
    for(Int_t i = 0; i < fNPrimaryHistograms; i++)
    {
      if ( !IsGeneratedParticlesAnalysisOn() ) continue;
      
      if ( !fFillOnlyPtHisto )
      {
        fhEPrimMC[i]  = new TH1F(Form("hEPrim_MC%s",ppname[i].Data()),
                                 Form("primary photon %s : E ",pptype[i].Data()),
                                 nptbins,ptmin,ptmax);
        fhEPrimMC[i]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEPrimMC[i]) ;
        
        
        fhEPrimMCAcc[i]  = new TH1F(Form("hEPrimAcc_MC%s",ppname[i].Data()),
                                    Form("primary photon %s in acceptance: E ",pptype[i].Data()),
                                    nptbins,ptmin,ptmax);
        fhEPrimMCAcc[i]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEPrimMCAcc[i]) ;
      }
      
      fhPtPrimMC[i]  = new TH1F(Form("hPtPrim_MC%s",ppname[i].Data()),
                                Form("primary photon %s : #it{p}_{T} ",pptype[i].Data()),
                                nptbins,ptmin,ptmax);
      fhPtPrimMC[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhPtPrimMC[i]) ;
      
      fhPtPrimMCAcc[i]  = new TH1F(Form("hPtPrimAcc_MC%s",ppname[i].Data()),
                                   Form("primary photon %s in acceptance: #it{p}_{T} ",pptype[i].Data()),
                                   nptbins,ptmin,ptmax);
      fhPtPrimMCAcc[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhPtPrimMCAcc[i]) ;
      
      fhYPrimMC[i]  = new TH2F(Form("hYPrim_MC%s",ppname[i].Data()),
                               Form("primary photon %s : Rapidity ",pptype[i].Data()),
                               nptbins,ptmin,ptmax,200,-2,2);
      fhYPrimMC[i]->SetYTitle("Rapidity");
      fhYPrimMC[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhYPrimMC[i]) ;
      
      fhEtaPrimMC[i]  = new TH2F(Form("hEtaPrim_MC%s",ppname[i].Data()),
                               Form("primary photon %s : #eta",pptype[i].Data()),
                               nptbins,ptmin,ptmax,200,-2,2);
      fhEtaPrimMC[i]->SetYTitle("#eta");
      fhEtaPrimMC[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhEtaPrimMC[i]) ;
      
      fhPhiPrimMC[i]  = new TH2F(Form("hPhiPrim_MC%s",ppname[i].Data()),
                                 Form("primary photon %s : #varphi ",pptype[i].Data()),
                                 nptbins,ptmin,ptmax,nphibins,0,TMath::TwoPi());
      fhPhiPrimMC[i]->SetYTitle("#varphi (rad)");
      fhPhiPrimMC[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhPhiPrimMC[i]) ;
      
      
      fhYPrimMCAcc[i]  = new TH2F(Form("hYPrimAcc_MC%s",ppname[i].Data()),
                                  Form("primary photon %s in acceptance: Rapidity ",pptype[i].Data()),
                                  nptbins,ptmin,ptmax,100,-1,1);
      fhYPrimMCAcc[i]->SetYTitle("Rapidity");
      fhYPrimMCAcc[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhYPrimMCAcc[i]) ;

      fhEtaPrimMCAcc[i]  = new TH2F(Form("hEtaPrimAcc_MC%s",ppname[i].Data()),
                                  Form("primary photon %s in acceptance: #eta ",pptype[i].Data()),
                                  nptbins,ptmin,ptmax,netabins,etamin,etamax);
      fhEtaPrimMCAcc[i]->SetYTitle("#eta");
      fhEtaPrimMCAcc[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhEtaPrimMCAcc[i]) ;
      
      fhPhiPrimMCAcc[i]  = new TH2F(Form("hPhiPrimAcc_MC%s",ppname[i].Data()),
                                    Form("primary photon %s in acceptance: #varphi ",pptype[i].Data()),
                                    nptbins,ptmin,ptmax,nphibins,phimin,phimax);
      fhPhiPrimMCAcc[i]->SetYTitle("#varphi (rad)");
      fhPhiPrimMCAcc[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhPhiPrimMCAcc[i]) ;
    }
    
    if ( fFillSSHistograms )
    {
      TString ptypess[] = { 
        "#gamma"                  , "#gamma_{prompt}"         , "#gamma_{fragmentation}",
        "#pi^{0}"                 , "#eta"                    , "e^{#pm}", "other",
        "#gamma->e^{#pm}"         , "#gamma_{prompt}->e^{#pm}", "#gamma_{fragmentation}->e^{#pm}",
        "#pi^{0}->#gamma->e^{#pm}", "#eta->#gamma->e^{#pm}" } ;
      
      TString pnamess[] = { 
        "Photon"    , "PhotonPrompt"    ,"PhotonFrag",
        "Pi0"       , "Eta"             ,"Electron"  , "Other",
        "PhotonConv", "PhotonPromptConv","PhotonFragConv",
        "Pi0Conv"   , "EtaConv" } ;
      
      Int_t nssTypes = fgkNssTypes;
      if ( !fSeparateConvertedDistributions ) nssTypes = 7;
      for(Int_t i = 0; i < nssTypes; i++)
      {
        if ( !fFillOnlyPtHisto )
        {
          fhMCELambda0[i]  = new TH2F(Form("hELambda0_MC%s",pnamess[i].Data()),
                                      Form("cluster from %s : E vs #sigma^{2}_{long}",ptypess[i].Data()),
                                      nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhMCELambda0[i]->SetYTitle("#sigma^{2}_{long}");
          fhMCELambda0[i]->SetXTitle("#it{E} (GeV)");
          outputContainer->Add(fhMCELambda0[i]) ;
        }
        
        fhMCPtLambda0[i]  = new TH2F(Form("hPtLambda0_MC%s",pnamess[i].Data()),
                                    Form("cluster from %s : #it{p}_{T} vs #sigma^{2}_{long}",ptypess[i].Data()),
                                    nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhMCPtLambda0[i]->SetYTitle("#sigma^{2}_{long}");
        fhMCPtLambda0[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhMCPtLambda0[i]) ;
        
        if ( !IsEmbedingAnalysisOn() && fCheckOverlaps )
        {
          for(Int_t iover = 0; iover < 3; iover++)
          {
            fhMCPtLambda0Overlaps[i][iover]  = new TH2F(Form("hPtLambda0_MC%s_Overlap%d",pnamess[i].Data(),iover),
                                                 Form("cluster from %s : #it{p}_{T} vs #sigma^{2}_{long}, N Overlaps = %d",ptypess[i].Data(),iover),
                                                 nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
            fhMCPtLambda0Overlaps[i][iover]->SetYTitle("#sigma^{2}_{long}");
            fhMCPtLambda0Overlaps[i][iover]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
            outputContainer->Add(fhMCPtLambda0Overlaps[i][iover]) ;
          }
        }
        
        if ( !fFillOnlyPtHisto )
        {
          fhMCELambda1[i]  = new TH2F(Form("hELambda1_MC%s",pnamess[i].Data()),
                                      Form("cluster from %s : E vs #sigma^{2}_{short}",ptypess[i].Data()),
                                      nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhMCELambda1[i]->SetYTitle("#sigma^{2}_{short}");
          fhMCELambda1[i]->SetXTitle("#it{E} (GeV)");
          outputContainer->Add(fhMCELambda1[i]) ;
        }
        
        if ( !fFillOnlySimpleSSHisto )
        {          
          fhMCNCellsE[i]  = new TH2F (Form("hNCellsE_MC%s",pnamess[i].Data()),
                                      Form("# of cells in cluster from %s vs E of clusters",ptypess[i].Data()),
                                      nptbins,ptmin,ptmax, nbins,nmin,nmax);
          fhMCNCellsE[i]->SetXTitle("#it{E} (GeV)");
          fhMCNCellsE[i]->SetYTitle("# of cells in cluster");
          outputContainer->Add(fhMCNCellsE[i]);
          
          fhMCEDispersion[i]  = new TH2F(Form("hEDispersion_MC%s",pnamess[i].Data()),
                                         Form("cluster from %s : E vs dispersion^{2}",ptypess[i].Data()),
                                         nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhMCEDispersion[i]->SetYTitle("D^{2}");
          fhMCEDispersion[i]->SetXTitle("#it{E} (GeV)");
          outputContainer->Add(fhMCEDispersion[i]) ;

          fhMCMaxCellDiffClusterE[i]  = new TH2F (Form("hMaxCellDiffClusterE_MC%s",pnamess[i].Data()),
                                                  Form("energy vs difference of cluster energy from %s - max cell energy / cluster energy, good clusters",ptypess[i].Data()),
                                                  nptbins,ptmin,ptmax, 500,0,1.);
          fhMCMaxCellDiffClusterE[i]->SetXTitle("#it{E}_{cluster} (GeV) ");
          fhMCMaxCellDiffClusterE[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCMaxCellDiffClusterE[i]);
          
          fhMCLambda0vsClusterMaxCellDiffE0[i]  = new TH2F(Form("hLambda0vsClusterMaxCellDiffE0_MC%s",pnamess[i].Data()),
                                                           Form("cluster from %s : #sigma^{2}_{long} vs fraction of energy carried by max cell, E < 2 GeV",ptypess[i].Data()),
                                                           ssbins,ssmin,ssmax,500,0,1.);
          fhMCLambda0vsClusterMaxCellDiffE0[i]->SetXTitle("#sigma^{2}_{long}");
          fhMCLambda0vsClusterMaxCellDiffE0[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCLambda0vsClusterMaxCellDiffE0[i]) ;
          
          fhMCLambda0vsClusterMaxCellDiffE2[i]  = new TH2F(Form("hLambda0vsClusterMaxCellDiffE2_MC%s",pnamess[i].Data()),
                                                           Form("cluster from %s : #sigma^{2}_{long} vs fraction of energy carried by max cell, 2< E < 6 GeV",ptypess[i].Data()),
                                                           ssbins,ssmin,ssmax,500,0,1.);
          fhMCLambda0vsClusterMaxCellDiffE2[i]->SetXTitle("#sigma^{2}_{long}");
          fhMCLambda0vsClusterMaxCellDiffE2[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCLambda0vsClusterMaxCellDiffE2[i]) ;
          
          fhMCLambda0vsClusterMaxCellDiffE6[i]  = new TH2F(Form("hLambda0vsClusterMaxCellDiffE6_MC%s",pnamess[i].Data()),
                                                           Form("cluster from %s : #sigma^{2}_{long} vs fraction of energy carried by max cell, #it{E} > 6 GeV",ptypess[i].Data()),
                                                           ssbins,ssmin,ssmax,500,0,1.);
          fhMCLambda0vsClusterMaxCellDiffE6[i]->SetXTitle("#sigma^{2}_{long}");
          fhMCLambda0vsClusterMaxCellDiffE6[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCLambda0vsClusterMaxCellDiffE6[i]) ;
          
          fhMCNCellsvsClusterMaxCellDiffE0[i]  = new TH2F(Form("hNCellsvsClusterMaxCellDiffE0_MC%s",pnamess[i].Data()),
                                                          Form("cluster from %s : N cells in cluster vs fraction of energy carried by max cell, E < 2 GeV",ptypess[i].Data()),
                                                          nbins/5,nmin,nmax/5,500,0,1.);
          fhMCNCellsvsClusterMaxCellDiffE0[i]->SetXTitle("N cells in cluster");
          fhMCNCellsvsClusterMaxCellDiffE0[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCNCellsvsClusterMaxCellDiffE0[i]) ;
          
          fhMCNCellsvsClusterMaxCellDiffE2[i]  = new TH2F(Form("hNCellsvsClusterMaxCellDiffE2_MC%s",pnamess[i].Data()),
                                                          Form("cluster from %s : N cells in cluster vs fraction of energy carried by max cell, 2< E < 6 GeV",ptypess[i].Data()),
                                                          nbins/5,nmin,nmax/5,500,0,1.);
          fhMCNCellsvsClusterMaxCellDiffE2[i]->SetXTitle("N cells in cluster");
          fhMCNCellsvsClusterMaxCellDiffE2[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCNCellsvsClusterMaxCellDiffE2[i]) ;
          
          fhMCNCellsvsClusterMaxCellDiffE6[i]  = new TH2F(Form("hNCellsvsClusterMaxCellDiffE6_MC%s",pnamess[i].Data()),
                                                          Form("cluster from %s : N cells in cluster vs fraction of energy carried by max cell, #it{E} > 6 GeV",ptypess[i].Data()),
                                                          nbins/5,nmin,nmax/5,500,0,1.);
          fhMCNCellsvsClusterMaxCellDiffE6[i]->SetXTitle("N cells in cluster");
          fhMCNCellsvsClusterMaxCellDiffE6[i]->SetYTitle("#it{E} (GeV)");
          outputContainer->Add(fhMCNCellsvsClusterMaxCellDiffE6[i]) ;
          
          if(GetCalorimeter()==kEMCAL)
          {
            fhMCEDispEta[i]  = new TH2F (Form("hEDispEtaE_MC%s",pnamess[i].Data()),
                                         Form("cluster from %s : #sigma^{2}_{#eta #eta} = #Sigma w_{i}(#eta_{i} - <#eta>)^{2}/ #Sigma w_{i} vs E",ptypess[i].Data()),
                                         nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
            fhMCEDispEta[i]->SetXTitle("#it{E} (GeV)");
            fhMCEDispEta[i]->SetYTitle("#sigma^{2}_{#eta #eta}");
            outputContainer->Add(fhMCEDispEta[i]);
            
            fhMCEDispPhi[i]  = new TH2F (Form("hEDispPhiE_MC%s",pnamess[i].Data()),
                                         Form("cluster from %s : #sigma^{2}_{#varphi #varphi} = #Sigma w_{i}(#varphi_{i} - <#varphi>)^{2} / #Sigma w_{i} vs E",ptypess[i].Data()),
                                         nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
            fhMCEDispPhi[i]->SetXTitle("#it{E} (GeV)");
            fhMCEDispPhi[i]->SetYTitle("#sigma^{2}_{#varphi #varphi}");
            outputContainer->Add(fhMCEDispPhi[i]);
            
            fhMCESumEtaPhi[i]  = new TH2F (Form("hESumEtaPhiE_MC%s",pnamess[i].Data()),
                                           Form("cluster from %s : #delta^{2}_{#eta #varphi} = #Sigma w_{i}(#varphi_{i} #eta_{i} ) / #Sigma w_{i} - <#varphi><#eta> vs E",ptypess[i].Data()),
                                           nptbins,ptmin,ptmax, 2*ssbins,-ssmax,ssmax);
            fhMCESumEtaPhi[i]->SetXTitle("#it{E} (GeV)");
            fhMCESumEtaPhi[i]->SetYTitle("#delta^{2}_{#eta #varphi}");
            outputContainer->Add(fhMCESumEtaPhi[i]);
            
            fhMCEDispEtaPhiDiff[i]  = new TH2F (Form("hEDispEtaPhiDiffE_MC%s",pnamess[i].Data()),
                                                Form("cluster from %s : #sigma^{2}_{#varphi #varphi} - #sigma^{2}_{#eta #eta} vs E",ptypess[i].Data()),
                                                nptbins,ptmin,ptmax,200,-10,10);
            fhMCEDispEtaPhiDiff[i]->SetXTitle("#it{E} (GeV)");
            fhMCEDispEtaPhiDiff[i]->SetYTitle("#sigma^{2}_{#varphi #varphi}-#sigma^{2}_{#eta #eta}");
            outputContainer->Add(fhMCEDispEtaPhiDiff[i]);
            
            fhMCESphericity[i]  = new TH2F (Form("hESphericity_MC%s",pnamess[i].Data()),
                                            Form("cluster from %s : (#sigma^{2}_{#varphi #varphi} - #sigma^{2}_{#eta #eta}) / (#sigma^{2}_{#eta #eta} + #sigma^{2}_{#varphi #varphi}) vs E",ptypess[i].Data()),
                                            nptbins,ptmin,ptmax, 200,-1,1);
            fhMCESphericity[i]->SetXTitle("#it{E} (GeV)");
            fhMCESphericity[i]->SetYTitle("s = (#sigma^{2}_{#varphi #varphi} - #sigma^{2}_{#eta #eta}) / (#sigma^{2}_{#eta #eta} + #sigma^{2}_{#varphi #varphi})");
            outputContainer->Add(fhMCESphericity[i]);
            
            for(Int_t ie = 0; ie < 7; ie++)
            {
              fhMCDispEtaDispPhi[ie][i] = new TH2F (Form("hMCDispEtaDispPhi_EBin%d_MC%s",ie,pnamess[i].Data()),
                                                    Form("cluster from %s : #sigma^{2}_{#varphi #varphi} vs #sigma^{2}_{#eta #eta} for %d < E < %d GeV",pnamess[i].Data(),bin[ie],bin[ie+1]),
                                                    ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
              fhMCDispEtaDispPhi[ie][i]->SetXTitle("#sigma^{2}_{#eta #eta}");
              fhMCDispEtaDispPhi[ie][i]->SetYTitle("#sigma^{2}_{#varphi #varphi}");
              outputContainer->Add(fhMCDispEtaDispPhi[ie][i]);
              
              fhMCLambda0DispEta[ie][i] = new TH2F (Form("hMCLambda0DispEta_EBin%d_MC%s",ie,pnamess[i].Data()),
                                                    Form("cluster from %s : #sigma^{2}_{long} vs #sigma^{2}_{#eta #eta} for %d < E < %d GeV",pnamess[i].Data(),bin[ie],bin[ie+1]),
                                                    ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
              fhMCLambda0DispEta[ie][i]->SetXTitle("#sigma^{2}_{long}");
              fhMCLambda0DispEta[ie][i]->SetYTitle("#sigma^{2}_{#varphi #varphi}");
              outputContainer->Add(fhMCLambda0DispEta[ie][i]);
              
              fhMCLambda0DispPhi[ie][i] = new TH2F (Form("hMCLambda0DispPhi_EBin%d_MC%s",ie,pnamess[i].Data()),
                                                    Form("cluster from %s :#sigma^{2}_{long} vs #sigma^{2}_{#varphi #varphi} for %d < E < %d GeV",pnamess[i].Data(),bin[ie],bin[ie+1]),
                                                    ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
              fhMCLambda0DispPhi[ie][i]->SetXTitle("#sigma^{2}_{long}");
              fhMCLambda0DispPhi[ie][i]->SetYTitle("#sigma^{2}_{#varphi #varphi}");
              outputContainer->Add(fhMCLambda0DispPhi[ie][i]);
            }
          }
        }
      }// loop
      
//      if(!IsEmbedingAnalysisOn())
//      {
//        fhMCPhotonELambda0NoOverlap  = new TH2F("hELambda0_MCPhoton_NoOverlap",
//                                                "cluster from Photon : E vs #sigma^{2}_{long}",
//                                                nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
//        fhMCPhotonELambda0NoOverlap->SetYTitle("#sigma^{2}_{long}");
//        fhMCPhotonELambda0NoOverlap->SetXTitle("#it{E} (GeV)");
//        outputContainer->Add(fhMCPhotonELambda0NoOverlap) ;
//        
//        fhMCPhotonELambda0TwoOverlap  = new TH2F("hELambda0_MCPhoton_TwoOverlap",
//                                                 "cluster from Photon : E vs #sigma^{2}_{long}",
//                                                 nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
//        fhMCPhotonELambda0TwoOverlap->SetYTitle("#sigma^{2}_{long}");
//        fhMCPhotonELambda0TwoOverlap->SetXTitle("#it{E} (GeV)");
//        outputContainer->Add(fhMCPhotonELambda0TwoOverlap) ;
//        
//        fhMCPhotonELambda0NOverlap  = new TH2F("hELambda0_MCPhoton_NOverlap",
//                                               "cluster from Photon : E vs #sigma^{2}_{long}",
//                                               nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
//        fhMCPhotonELambda0NOverlap->SetYTitle("#sigma^{2}_{long}");
//        fhMCPhotonELambda0NOverlap->SetXTitle("#it{E} (GeV)");
//        outputContainer->Add(fhMCPhotonELambda0NOverlap) ;
//      } // No embedding
      
      
      if ( IsEmbedingAnalysisOn() )
      {
        fhEmbeddedSignalFractionEnergy  = new TH2F
        ("hEmbeddedSignal_FractionEnergy",
         "Energy Fraction of embedded signal versus cluster energy",
         nptbins,ptmin,ptmax,100,0.,1.);
        fhEmbeddedSignalFractionEnergy->SetYTitle("#it{E}_{MC} / #it{E}_{MC+BKG}");
        fhEmbeddedSignalFractionEnergy->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbeddedSignalFractionEnergy) ;
        
        if ( IsHighMultiplicityAnalysisOn() )
        {
          fhEmbeddedSignalFractionEnergyPerCentrality  = new TH3F
          ("hEmbeddedSignal_FractionEnergy_PerCentrality",
           "Energy Fraction of embedded signal versus cluster energy and centrality",
            ptBinsArray.GetSize() - 1,   ptBinsArray.GetArray(),
            frBinsArray.GetSize() - 1,   frBinsArray.GetArray(), 
           cenBinsArray.GetSize() - 1,  cenBinsArray.GetArray());
          fhEmbeddedSignalFractionEnergyPerCentrality->SetYTitle("#it{E}_{MC} / #it{E}_{MC+BKG}");
          fhEmbeddedSignalFractionEnergyPerCentrality->SetXTitle("#it{E} (GeV)");
          fhEmbeddedSignalFractionEnergyPerCentrality->SetZTitle("Centrality (%)");
          outputContainer->Add(fhEmbeddedSignalFractionEnergyPerCentrality) ;
        }
        
        fhEmbeddedPhotonFractionEnergy  = new TH2F
        ("hEmbeddedPhoton_FractionEnergy",
         "Energy Fraction of embedded photons versus cluster energy",
         nptbins,ptmin,ptmax,100,0.,1.);
        fhEmbeddedPhotonFractionEnergy->SetYTitle("#it{E}_{MC} / #it{E}_{MC+BKG}");
        fhEmbeddedPhotonFractionEnergy->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbeddedPhotonFractionEnergy) ;
        
        fhEmbeddedPhotonFractionEnergyM02  = new TH3F
        ("hEmbeddedPhoton_FractionEnergy_M02",
         "Energy Fraction of embedded photons versus cluster energy and #sigma_{long}^{2}",
         ptBinsArray.GetSize() - 1,  ptBinsArray.GetArray(),
         frBinsArray.GetSize() - 1,  frBinsArray.GetArray(), 
         ssBinsArray.GetSize() - 1,  ssBinsArray.GetArray());
        fhEmbeddedPhotonFractionEnergyM02->SetZTitle("#sigma_{long}^{2}");
        fhEmbeddedPhotonFractionEnergyM02->SetYTitle("#it{E}_{MC} / #it{E}_{MC+BKG}");
        fhEmbeddedPhotonFractionEnergyM02->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbeddedPhotonFractionEnergyM02) ;
        
        fhEmbeddedPi0FractionEnergy  = new TH2F
        ("hEmbeddedPi0_FractionEnergy",
         "Energy Fraction of embedded #pi^{0} versus cluster energy",
         nptbins,ptmin,ptmax,100,0.,1.);
        fhEmbeddedPi0FractionEnergy->SetYTitle("#it{E}_{MC} / #it{E}_{MC+BKG}");
        fhEmbeddedPi0FractionEnergy->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbeddedPi0FractionEnergy) ;
        
        fhEmbeddedPi0FractionEnergyM02  = new TH3F
        ("hEmbeddedPi0_FractionEnergy_M02",
         "Energy Fraction of embedded #pi^{0} versus cluster energy and #sigma_{long}^{2}",
         ptBinsArray.GetSize() - 1,  ptBinsArray.GetArray(),
         frBinsArray.GetSize() - 1,  frBinsArray.GetArray(), 
         ssBinsArray.GetSize() - 1,  ssBinsArray.GetArray());
        fhEmbeddedPi0FractionEnergyM02->SetZTitle("#sigma_{long}^{2}");
        fhEmbeddedPi0FractionEnergyM02->SetYTitle("#it{E}_{MC} / #it{E}_{MC+BKG}");
        fhEmbeddedPi0FractionEnergyM02->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbeddedPi0FractionEnergyM02) ;
      }// embedded histograms
    }// Fill SS MC histograms
    
    if ( fFillConversionVertexHisto )
    {
      fhMCConversionVertex = new TH2F("hMCPhotonConversionVertex","cluster from converted photon, #it{p}_{T} vs vertex distance",
                                      nptbins,ptmin,ptmax,500,0,500);
      fhMCConversionVertex->SetYTitle("#it{R} (cm)");
      fhMCConversionVertex->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhMCConversionVertex) ;
      
      if(GetCalorimeter() == kEMCAL &&  GetFirstSMCoveredByTRD() >= 0)
      {
        fhMCConversionVertexTRD = new TH2F("hMCPhotonConversionVertexTRD","cluster from converted photon, #it{p}_{T} vs vertex distance, SM covered by TRD",
                                           nptbins,ptmin,ptmax,500,0,500);
        fhMCConversionVertexTRD->SetYTitle("#it{R} (cm)");
        fhMCConversionVertexTRD->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhMCConversionVertexTRD) ;
      }
      
      if(fFillSSHistograms)
      {
        TString region[] = {"ITS","TPC","TRD","TOF","Top EMCal","In EMCal"};
        for(Int_t iR = 0; iR < 6; iR++)
        {
          fhMCConversionLambda0Rcut[iR] = new TH2F(Form("hMCPhotonConversionLambda0_R%d",iR),
                                                   Form("cluster from converted photon, #it{p}_{T} vs #sigma^{2}_{long}, conversion in %s",region[iR].Data()),
                                                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhMCConversionLambda0Rcut[iR]->SetYTitle("#sigma^{2}_{long}");
          fhMCConversionLambda0Rcut[iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
          outputContainer->Add(fhMCConversionLambda0Rcut[iR]) ;
          
          fhMCConversionLambda1Rcut[iR] = new TH2F(Form("hMCPhotonConversionLambda1_R%d",iR),
                                                   Form("cluster from converted photon, #it{p}_{T} vs #sigma^{2}_{short}, conversion in %s",region[iR].Data()),
                                                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhMCConversionLambda1Rcut[iR]->SetYTitle("#sigma^{2}_{short}");
          fhMCConversionLambda1Rcut[iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
          outputContainer->Add(fhMCConversionLambda1Rcut[iR]) ;
        } // R cut
        
        
        if(GetCalorimeter() == kEMCAL &&  GetFirstSMCoveredByTRD() >= 0)
        {
          for(Int_t iR = 0; iR < 6; iR++)
          {
            fhMCConversionLambda0RcutTRD[iR] = new TH2F(Form("hMCPhotonConversionLambda0TRD_R%d",iR),
                                                        Form("cluster from converted photon, #it{p}_{T} vs #sigma^{2}_{long}, conversion in %s, SM covered by TRD",region[iR].Data()),
                                                        nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
            fhMCConversionLambda0RcutTRD[iR]->SetYTitle("#sigma^{2}_{long}");
            fhMCConversionLambda0RcutTRD[iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
            outputContainer->Add(fhMCConversionLambda0RcutTRD[iR]) ;
            
            fhMCConversionLambda1RcutTRD[iR] = new TH2F(Form("hMCPhotonConversionLambda1TRD_R%d",iR),
                                                        Form("cluster from converted photon, #it{p}_{T} vs #sigma^{2}_{short}, conversion in %s, SM covered by TRD",region[iR].Data()),
                                                        nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
            fhMCConversionLambda1RcutTRD[iR]->SetYTitle("#sigma^{2}_{short}");
            fhMCConversionLambda1RcutTRD[iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
            outputContainer->Add(fhMCConversionLambda1RcutTRD[iR]) ;
          } // R cut
        }
        
        //      if(GetCalorimeter() == kEMCAL && fFillEMCALRegionSSHistograms)
        //      {
        //        for(Int_t ieta = 0; ieta < 4; ieta++) 
        //        {  
        //          for(Int_t iphi = 0; iphi < 3; iphi++) 
        //          {
        //            for(Int_t iR = 0; iR < 6; iR++) 
        //            {
        //              fhLam0EMCALRegionMCConvRcut[ieta][iphi][iR] = 
        //              new TH2F(Form("hMCPhotonConversionLambda0_R%d_eta%d_phi%d",iR,ieta,iphi),
        //                       Form("cluster from converted photon, #it{p}_{T} vs #sigma^{2}_{long}, conversion in %s, region eta %d, phi %d",region[iR].Data(),ieta,iphi),
        //                       nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        //              fhLam0EMCALRegionMCConvRcut[ieta][iphi][iR]->SetYTitle("#sigma^{2}_{long}");
        //              fhLam0EMCALRegionMCConvRcut[ieta][iphi][iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        //              outputContainer->Add(fhLam0EMCALRegionMCConvRcut[ieta][iphi][iR]) ;
        //              
        //              if(GetFirstSMCoveredByTRD() >= 0)
        //              {
        //                fhLam0EMCALRegionTRDMCConvRcut[ieta][iphi][iR] = 
        //                new TH2F(Form("hMCPhotonConversionLambda0TRD_R%d_eta%d_phi%d",iR,ieta,iphi),
        //                         Form("cluster from converted photon, #it{p}_{T} vs #sigma^{2}_{long}, conversion in %s, region eta %d, phi %d, SM covered by TRD",region[iR].Data(),ieta,iphi),
        //                         nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        //                fhLam0EMCALRegionTRDMCConvRcut[ieta][iphi][iR]->SetYTitle("#sigma^{2}_{long}");
        //                fhLam0EMCALRegionTRDMCConvRcut[ieta][iphi][iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        //                outputContainer->Add(fhLam0EMCALRegionTRDMCConvRcut[ieta][iphi][iR]) ;
        //              } // TRD
        //
        //            } // iR
        //          } // iphi 
        //        } // ieta
        //      } // regions in EMCal
      } // shower shape
    } // conversion vertex
  } // Histos with MC
  
  if ( fStudyActivityNearCluster )
  {
    TString caseTitle[] = {"","CleanCluster","MergedClusterHijingBkg","MergedClusterNotHijingBkg","MergedClusterHijingAndOtherBkg","MergedCluster"};
    Int_t ncases = 1;
    if ( IsDataMC() && IsStudyClusterOverlapsPerGeneratorOn() ) ncases = 6;
    
    for(Int_t icase = 0; icase < ncases; icase++)
    {
      fhLocalRegionClusterEtaPhi[icase]  = new TH2F
      (Form("hLocalRegionClusterEtaPhi%s",caseTitle[icase].Data()),
       "cluster,#it{E} > 0.5 GeV, #eta vs #varphi",netabins,etamin,etamax,nphibins,phimin,phimax);
      fhLocalRegionClusterEtaPhi[icase]->SetYTitle("#varphi (rad)");
      fhLocalRegionClusterEtaPhi[icase]->SetXTitle("#eta");
      outputContainer->Add(fhLocalRegionClusterEtaPhi[icase]) ;
      
      fhLocalRegionClusterEnergySum[icase] = new TH2F 
      (Form("hLocalRegionClusterEnergySum%s",caseTitle[icase].Data()),
       "Sum of cluster energy around trigger cluster #it{E} with R=0.2", 
       nptbins,ptmin,ptmax, 200,0,100);
      fhLocalRegionClusterEnergySum[icase]->SetXTitle("#it{E} (GeV)");
      fhLocalRegionClusterEnergySum[icase]->SetYTitle("#Sigma #it{E} (GeV)");
      outputContainer->Add(fhLocalRegionClusterEnergySum[icase]);    
      
      fhLocalRegionClusterMultiplicity[icase] = new TH2F 
      (Form("hLocalRegionClusterMultiplicity%s",caseTitle[icase].Data()),
       "Cluster multiplicity around trigger cluster #it{E} with R=0.2", 
       nptbins,ptmin,ptmax, 200,0,200);
      fhLocalRegionClusterMultiplicity[icase]->SetXTitle("#it{E} (GeV)");
      fhLocalRegionClusterMultiplicity[icase]->SetYTitle("Multiplicity");
      outputContainer->Add(fhLocalRegionClusterMultiplicity[icase]);    
      
      if(IsHighMultiplicityAnalysisOn())
      {
        fhLocalRegionClusterEnergySumPerCentrality[icase] = new TH2F 
        (Form("hLocalRegionClusterEnergySumPerCentrality%s",caseTitle[icase].Data()),
         "Sum of cluster energy around trigger cluster vs centrality with R=0.2", 
         100,0,100, 200,0,100);
        fhLocalRegionClusterEnergySumPerCentrality[icase]->SetXTitle("Centrality");
        fhLocalRegionClusterEnergySumPerCentrality[icase]->SetYTitle("#Sigma #it{E} (GeV)");
        outputContainer->Add(fhLocalRegionClusterEnergySumPerCentrality[icase]);    
        
        fhLocalRegionClusterMultiplicityPerCentrality[icase] = new TH2F 
        (Form("hLocalRegionClusterMultiplicityPerCentrality%s",caseTitle[icase].Data()),
         "Cluster multiplicity around trigger cluster vs centrality with R=0.2", 
         100,0,100, 200,0,200);
        fhLocalRegionClusterMultiplicityPerCentrality[icase]->SetXTitle("Centrality");
        fhLocalRegionClusterMultiplicityPerCentrality[icase]->SetYTitle("Multiplicity");
        outputContainer->Add(fhLocalRegionClusterMultiplicityPerCentrality[icase]);        
      }
      
      //
      // Select only single pi0 decay clusters from non hijing generator
      //
      if(IsDataMC())
      {
        fhLocalRegionClusterEnergySumMCPi0Decay[icase] = new TH2F 
        (Form("hLocalRegionClusterEnergySum%s_MCPi0Decay",caseTitle[icase].Data()),
         "Sum of cluster energy around trigger cluster #it{E} with R=0.2", 
         nptbins,ptmin,ptmax, 200,0,100);
        fhLocalRegionClusterEnergySumMCPi0Decay[icase]->SetXTitle("#it{E} (GeV)");
        fhLocalRegionClusterEnergySumMCPi0Decay[icase]->SetYTitle("#Sigma #it{E} (GeV)");
        outputContainer->Add(fhLocalRegionClusterEnergySumMCPi0Decay[icase]);    
        
        fhLocalRegionClusterMultiplicityMCPi0Decay[icase] = new TH2F 
        (Form("hLocalRegionClusterMultiplicity%s_MCPi0Decay",caseTitle[icase].Data()),
         "Cluster multiplicity around trigger cluster #it{E} with R=0.2", 
         nptbins,ptmin,ptmax, 200,0,200);
        fhLocalRegionClusterMultiplicityMCPi0Decay[icase]->SetXTitle("#it{E} (GeV)");
        fhLocalRegionClusterMultiplicityMCPi0Decay[icase]->SetYTitle("Multiplicity");
        outputContainer->Add(fhLocalRegionClusterMultiplicityMCPi0Decay[icase]);    
        
        if(IsHighMultiplicityAnalysisOn())
        {
          fhLocalRegionClusterEnergySumPerCentralityMCPi0Decay[icase] = new TH2F 
          (Form("hLocalRegionClusterEnergySumPerCentrality%s_MCPi0Decay",caseTitle[icase].Data()),
           "Sum of cluster energy around trigger cluster vs centrality with R=0.2", 
           100,0,100, 200,0,100);
          fhLocalRegionClusterEnergySumPerCentralityMCPi0Decay[icase]->SetXTitle("Centrality");
          fhLocalRegionClusterEnergySumPerCentralityMCPi0Decay[icase]->SetYTitle("#Sigma #it{E} (GeV)");
          outputContainer->Add(fhLocalRegionClusterEnergySumPerCentralityMCPi0Decay[icase]);    
          
          fhLocalRegionClusterMultiplicityPerCentralityMCPi0Decay[icase] = new TH2F 
          (Form("hLocalRegionClusterMultiplicityPerCentrality%s_MCPi0Decay",caseTitle[icase].Data()),
           "Cluster multiplicity around trigger cluster vs centrality with R=0.2", 
           100,0,100, 200,0,200);
          fhLocalRegionClusterMultiplicityPerCentralityMCPi0Decay[icase]->SetXTitle("Centrality");
          fhLocalRegionClusterMultiplicityPerCentralityMCPi0Decay[icase]->SetYTitle("Multiplicity");
          outputContainer->Add(fhLocalRegionClusterMultiplicityPerCentralityMCPi0Decay[icase]);        
        }
      }
      
      if(IsStudyClusterOverlapsPerGeneratorOn() && IsDataMC())
      {
        fhLocalRegionClusterEnergySumHijing[icase] = new TH2F 
        (Form("hLocalRegionClusterEnergySumHijing%s",caseTitle[icase].Data()),
         "Sum of cluster energy (HIJING) around trigger cluster #it{E} with R=0.2", 
         nptbins,ptmin,ptmax, 200,0,100);
        fhLocalRegionClusterEnergySumHijing[icase]->SetXTitle("#it{E} (GeV)");
        fhLocalRegionClusterEnergySumHijing[icase]->SetYTitle("#Sigma #it{E} (GeV)");
        outputContainer->Add(fhLocalRegionClusterEnergySumHijing[icase]);    
        
        fhLocalRegionClusterMultiplicityHijing[icase] = new TH2F 
        (Form("hLocalRegionClusterMultiplicityHijing%s",caseTitle[icase].Data()),
         "Cluster multiplicity (HIJING) around trigger cluster #it{E} with R=0.2", 
         nptbins,ptmin,ptmax, 200,0,200);
        fhLocalRegionClusterMultiplicityHijing[icase]->SetXTitle("#it{E} (GeV)");
        fhLocalRegionClusterMultiplicityHijing[icase]->SetYTitle("Multiplicity");
        outputContainer->Add(fhLocalRegionClusterMultiplicityHijing[icase]);    
        
        fhLocalRegionClusterEnergySumAdded[icase] = new TH2F 
        (Form("hLocalRegionClusterEnergySumAdded%s",caseTitle[icase].Data()),
         "Sum of cluster energy (not HIJING) around trigger cluster #it{E} with R=0.2", 
         nptbins,ptmin,ptmax, 200,0,100);
        fhLocalRegionClusterEnergySumAdded[icase]->SetXTitle("#it{E} (GeV)");
        fhLocalRegionClusterEnergySumAdded[icase]->SetYTitle("#Sigma #it{E} (GeV)");
        outputContainer->Add(fhLocalRegionClusterEnergySumAdded[icase]);    
        
        fhLocalRegionClusterMultiplicityAdded[icase] = new TH2F 
        (Form("hLocalRegionClusterMultiplicityAdded%s",caseTitle[icase].Data()),
         "Cluster multiplicity (not HIJING) around trigger cluster #it{E} with R=0.2", 
         nptbins,ptmin,ptmax, 200,0,200);
        fhLocalRegionClusterMultiplicityAdded[icase]->SetXTitle("#it{E} (GeV)");
        fhLocalRegionClusterMultiplicityAdded[icase]->SetYTitle("Multiplicity");
        outputContainer->Add(fhLocalRegionClusterMultiplicityAdded[icase]);    
        
        
        if(IsHighMultiplicityAnalysisOn())
        {
          fhLocalRegionClusterEnergySumPerCentralityHijing[icase] = new TH2F 
          (Form("hLocalRegionClusterEnergySumPerCentralityHijing%s",caseTitle[icase].Data()),
           "Sum of cluster energy (HIJING) around trigger cluster vs centrality with R=0.2", 
           100,0,100, 200,0,100);
          fhLocalRegionClusterEnergySumPerCentralityHijing[icase]->SetXTitle("Centrality");
          fhLocalRegionClusterEnergySumPerCentralityHijing[icase]->SetYTitle("#Sigma #it{E} (GeV)");
          outputContainer->Add(fhLocalRegionClusterEnergySumPerCentralityHijing[icase]);    
          
          fhLocalRegionClusterMultiplicityPerCentralityHijing[icase] = new TH2F 
          (Form("hLocalRegionClusterMultiplicityPerCentralityHijing%s",caseTitle[icase].Data()),
           "Cluster multiplicity (HIJING) around trigger cluster vs centrality with R=0.2", 
           100,0,100, 200,0,200);
          fhLocalRegionClusterMultiplicityPerCentralityHijing[icase]->SetXTitle("Centrality");
          fhLocalRegionClusterMultiplicityPerCentralityHijing[icase]->SetYTitle("Multiplicity");
          outputContainer->Add(fhLocalRegionClusterMultiplicityPerCentralityHijing[icase]);        
          
          
          fhLocalRegionClusterEnergySumPerCentralityAdded[icase] = new TH2F 
          (Form("hLocalRegionClusterEnergySumPerCentralityAdded%s",caseTitle[icase].Data()),
           "Sum of cluster energy (not HIJING) around trigger cluster vs centrality with R=0.2", 
           100,0,100, 200,0,100);
          fhLocalRegionClusterEnergySumPerCentralityAdded[icase]->SetXTitle("Centrality");
          fhLocalRegionClusterEnergySumPerCentralityAdded[icase]->SetYTitle("#Sigma #it{E} (GeV)");
          outputContainer->Add(fhLocalRegionClusterEnergySumPerCentralityAdded[icase]);    
          
          fhLocalRegionClusterMultiplicityPerCentralityAdded[icase] = new TH2F 
          (Form("hLocalRegionClusterMultiplicityPerCentralityAdded%s",caseTitle[icase].Data()),
           "Cluster multiplicity (not HIJING) around trigger cluster vs centrality with R=0.2", 
           100,0,100, 200,0,200);
          fhLocalRegionClusterMultiplicityPerCentralityAdded[icase]->SetXTitle("Centrality");
          fhLocalRegionClusterMultiplicityPerCentralityAdded[icase]->SetYTitle("Multiplicity");
          outputContainer->Add(fhLocalRegionClusterMultiplicityPerCentralityAdded[icase]);        
        }
        
        //
        // Select only single pi0 decay clusters from non hijing generator
        //
        
        fhLocalRegionClusterEnergySumHijingMCPi0Decay[icase] = new TH2F 
        (Form("hLocalRegionClusterEnergySumHijing%s_MCPi0Decay",caseTitle[icase].Data()),
         "Sum of cluster energy (HIJING) around trigger cluster #it{E} with R=0.2", 
         nptbins,ptmin,ptmax, 200,0,100);
        fhLocalRegionClusterEnergySumHijingMCPi0Decay[icase]->SetXTitle("#it{E} (GeV)");
        fhLocalRegionClusterEnergySumHijingMCPi0Decay[icase]->SetYTitle("#Sigma #it{E} (GeV)");
        outputContainer->Add(fhLocalRegionClusterEnergySumHijingMCPi0Decay[icase]);    
        
        fhLocalRegionClusterMultiplicityHijingMCPi0Decay[icase] = new TH2F 
        (Form("hLocalRegionClusterMultiplicityHijing%s_MCPi0Decay",caseTitle[icase].Data()),
         "Cluster multiplicity (HIJING) around trigger cluster #it{E} with R=0.2", 
         nptbins,ptmin,ptmax, 200,0,200);
        fhLocalRegionClusterMultiplicityHijingMCPi0Decay[icase]->SetXTitle("#it{E} (GeV)");
        fhLocalRegionClusterMultiplicityHijingMCPi0Decay[icase]->SetYTitle("Multiplicity");
        outputContainer->Add(fhLocalRegionClusterMultiplicityHijingMCPi0Decay[icase]);    
        
        fhLocalRegionClusterEnergySumAddedMCPi0Decay[icase] = new TH2F 
        (Form("hLocalRegionClusterEnergySumAdded%s_MCPi0Decay",caseTitle[icase].Data()),
         "Sum of cluster energy (not HIJING) around trigger cluster #it{E} with R=0.2", 
         nptbins,ptmin,ptmax, 200,0,100);
        fhLocalRegionClusterEnergySumAddedMCPi0Decay[icase]->SetXTitle("#it{E} (GeV)");
        fhLocalRegionClusterEnergySumAddedMCPi0Decay[icase]->SetYTitle("#Sigma #it{E} (GeV)");
        outputContainer->Add(fhLocalRegionClusterEnergySumAddedMCPi0Decay[icase]);    
        
        fhLocalRegionClusterMultiplicityAddedMCPi0Decay[icase] = new TH2F 
        (Form("hLocalRegionClusterMultiplicityAdded%s_MCPi0Decay",caseTitle[icase].Data()),
         "Cluster multiplicity (not HIJING) around trigger cluster #it{E} with R=0.2", 
         nptbins,ptmin,ptmax, 200,0,200);
        fhLocalRegionClusterMultiplicityAddedMCPi0Decay[icase]->SetXTitle("#it{E} (GeV)");
        fhLocalRegionClusterMultiplicityAddedMCPi0Decay[icase]->SetYTitle("Multiplicity");
        outputContainer->Add(fhLocalRegionClusterMultiplicityAddedMCPi0Decay[icase]);    
        
        
        if(IsHighMultiplicityAnalysisOn())
        {
          fhLocalRegionClusterEnergySumPerCentralityHijingMCPi0Decay[icase] = new TH2F 
          (Form("hLocalRegionClusterEnergySumPerCentralityHijing%s_MCPi0Decay",caseTitle[icase].Data()),
           "Sum of cluster energy (HIJING) around trigger cluster vs centrality with R=0.2", 
           100,0,100, 200,0,100);
          fhLocalRegionClusterEnergySumPerCentralityHijingMCPi0Decay[icase]->SetXTitle("Centrality");
          fhLocalRegionClusterEnergySumPerCentralityHijingMCPi0Decay[icase]->SetYTitle("#Sigma #it{E} (GeV)");
          outputContainer->Add(fhLocalRegionClusterEnergySumPerCentralityHijingMCPi0Decay[icase]);    
          
          fhLocalRegionClusterMultiplicityPerCentralityHijingMCPi0Decay[icase] = new TH2F 
          (Form("hLocalRegionClusterMultiplicityPerCentralityHijing%s_MCPi0Decay",caseTitle[icase].Data()),
           "Cluster multiplicity (HIJING) around trigger cluster vs centrality with R=0.2", 
           100,0,100, 200,0,200);
          fhLocalRegionClusterMultiplicityPerCentralityHijingMCPi0Decay[icase]->SetXTitle("Centrality");
          fhLocalRegionClusterMultiplicityPerCentralityHijingMCPi0Decay[icase]->SetYTitle("Multiplicity");
          outputContainer->Add(fhLocalRegionClusterMultiplicityPerCentralityHijingMCPi0Decay[icase]);        
          
          
          fhLocalRegionClusterEnergySumPerCentralityAddedMCPi0Decay[icase] = new TH2F 
          (Form("hLocalRegionClusterEnergySumPerCentralityAdded%s_MCPi0Decay",caseTitle[icase].Data()),
           "Sum of cluster energy (not HIJING) around trigger cluster vs centrality with R=0.2", 
           100,0,100, 200,0,100);
          fhLocalRegionClusterEnergySumPerCentralityAddedMCPi0Decay[icase]->SetXTitle("Centrality");
          fhLocalRegionClusterEnergySumPerCentralityAddedMCPi0Decay[icase]->SetYTitle("#Sigma #it{E} (GeV)");
          outputContainer->Add(fhLocalRegionClusterEnergySumPerCentralityAddedMCPi0Decay[icase]);    
          
          fhLocalRegionClusterMultiplicityPerCentralityAddedMCPi0Decay[icase] = new TH2F 
          (Form("hLocalRegionClusterMultiplicityPerCentralityAdded%s_MCPi0Decay",caseTitle[icase].Data()),
           "Cluster multiplicity (not HIJING) around trigger cluster vs centrality with R=0.2", 
           100,0,100, 200,0,200);
          fhLocalRegionClusterMultiplicityPerCentralityAddedMCPi0Decay[icase]->SetXTitle("Centrality");
          fhLocalRegionClusterMultiplicityPerCentralityAddedMCPi0Decay[icase]->SetYTitle("Multiplicity");
          outputContainer->Add(fhLocalRegionClusterMultiplicityPerCentralityAddedMCPi0Decay[icase]);        
        }
        
      }
    }
    
    if(IsDataMC())
    {
      fhLocalRegionClusterEnergySumHijing2 = new TH2F 
      ("hLocalRegionClusterEnergySumHijing2",
       "Sum of cluster energy (HIJING) around trigger cluster #it{E} with R=0.2", 
       nptbins,ptmin,ptmax, 200,0,100);
      fhLocalRegionClusterEnergySumHijing2->SetXTitle("#it{E} (GeV)");
      fhLocalRegionClusterEnergySumHijing2->SetYTitle("#Sigma #it{E} (GeV)");
      outputContainer->Add(fhLocalRegionClusterEnergySumHijing2);    
      
      fhLocalRegionClusterMultiplicityHijing2 = new TH2F 
      ("hLocalRegionClusterMultiplicityHijing2",
       "Cluster multiplicity (HIJING) around trigger cluster #it{E} with R=0.2", 
       nptbins,ptmin,ptmax, 200,0,200);
      fhLocalRegionClusterMultiplicityHijing2->SetXTitle("#it{E} (GeV)");
      fhLocalRegionClusterMultiplicityHijing2->SetYTitle("Multiplicity");
      outputContainer->Add(fhLocalRegionClusterMultiplicityHijing2); 
      
      if(IsHighMultiplicityAnalysisOn())
      {
        fhLocalRegionClusterEnergySumPerCentralityHijing2 = new TH2F 
        ("hLocalRegionClusterEnergySumPerCentralityHijing2",
         "Sum of cluster energy (HIJING) around trigger cluster vs centrality with R=0.2", 
         100,0,100, 200,0,100);
        fhLocalRegionClusterEnergySumPerCentralityHijing2->SetXTitle("Centrality");
        fhLocalRegionClusterEnergySumPerCentralityHijing2->SetYTitle("#Sigma #it{E} (GeV)");
        outputContainer->Add(fhLocalRegionClusterEnergySumPerCentralityHijing2);    
        
        fhLocalRegionClusterMultiplicityPerCentralityHijing2 = new TH2F 
        ("hLocalRegionClusterMultiplicityPerCentralityHijing2",
         "Cluster multiplicity (HIJING) around trigger cluster vs centrality with R=0.2", 
         100,0,100, 200,0,200);
        fhLocalRegionClusterMultiplicityPerCentralityHijing2->SetXTitle("Centrality");
        fhLocalRegionClusterMultiplicityPerCentralityHijing2->SetYTitle("Multiplicity");
        outputContainer->Add(fhLocalRegionClusterMultiplicityPerCentralityHijing2);     
      }
    }
    //    fhDistanceAddedPhotonAddedPrimarySignal = new TH2F
    //    ("hDistanceAddedPhotonAddedPrimarySignal", "Distance added #gamma to primary particle from added generator"
    //    ,nptbins,ptmin,ptmax,100,0,0.4);
    //    fhDistanceAddedPhotonAddedPrimarySignal->SetYTitle("#it{R}");
    //    fhDistanceAddedPhotonAddedPrimarySignal->SetXTitle("#it{p}_{T}");
    //    outputContainer->Add(fhDistanceAddedPhotonAddedPrimarySignal) ;
    //    
    //    fhDistanceHijingPhotonAddedPrimarySignal = new TH2F
    //    ("hDistanceHijingPhotonAddedPrimarySignal", "Distance Hijing #gamma to primary particle from added generator"
    //     ,nptbins,ptmin,ptmax,100,0,0.4);
    //    fhDistanceHijingPhotonAddedPrimarySignal->SetYTitle("#it{R}");
    //    fhDistanceHijingPhotonAddedPrimarySignal->SetXTitle("#it{p}_{T}");
    //    outputContainer->Add(fhDistanceHijingPhotonAddedPrimarySignal) ;
    //
    //    fhDistanceAddedPhotonAddedSecondarySignal = new TH2F
    //    ("hDistanceAddedPhotonAddedSecondarySignal", "Distance added #gamma to secondary particle from added generator"
    //     ,nptbins,ptmin,ptmax,100,0,0.4);
    //    fhDistanceAddedPhotonAddedSecondarySignal->SetYTitle("#it{R}");
    //    fhDistanceAddedPhotonAddedSecondarySignal->SetXTitle("#it{p}_{T}");
    //    outputContainer->Add(fhDistanceAddedPhotonAddedSecondarySignal) ;
    //    
    //    fhDistanceHijingPhotonAddedSecondarySignal = new TH2F
    //    ("hDistanceHijingPhotonAddedSecondarySignal", "Distance Hijing #gamma to secondary particle from added generator"
    //     ,nptbins,ptmin,ptmax,100,0,0.4);
    //    fhDistanceHijingPhotonAddedSecondarySignal->SetYTitle("#it{R}");
    //    fhDistanceHijingPhotonAddedSecondarySignal->SetXTitle("#it{p}_{T}");
    //    outputContainer->Add(fhDistanceHijingPhotonAddedSecondarySignal) ;
    //
    //    fhDistanceAddedPhotonHijingSecondary = new TH2F
    //    ("hDistanceAddedPhotonHijingSecondary", "Distance added #gamma to secondary particle from hijing"
    //     ,nptbins,ptmin,ptmax,100,0,0.4);
    //    fhDistanceAddedPhotonHijingSecondary->SetYTitle("#it{R}");
    //    fhDistanceAddedPhotonHijingSecondary->SetXTitle("#it{p}_{T}");
    //    outputContainer->Add(fhDistanceAddedPhotonHijingSecondary) ;
    
    
    fhDistance2AddedSignals = new TH2F
    ("hDistance2AddedSignals", "Distance added signals"
     ,nptbins,ptmin,ptmax,100,0,0.4);
    fhDistance2AddedSignals->SetYTitle("#it{R}");
    fhDistance2AddedSignals->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhDistance2AddedSignals) ;
    
    fhDistance2Hijing = new TH2F
    ("hDistance2Hijing", "Distance 2 hijing clusters"
     ,nptbins,ptmin,ptmax,100,0,0.4);
    fhDistance2Hijing->SetYTitle("#it{R}");
    fhDistance2Hijing->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhDistance2Hijing) ;
    
    fhDistanceAddedSignalsHijing = new TH2F
    ("hDistanceAddedSignalsHijing", "Distance added signals to hijing"
     ,nptbins,ptmin,ptmax,100,0,0.4);
    fhDistanceAddedSignalsHijing->SetYTitle("#it{R}");
    fhDistanceAddedSignalsHijing->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhDistanceAddedSignalsHijing) ;
  }

  if ( IsStudyClusterOverlapsPerGeneratorOn() && IsDataMC() )
  {
     
    TString mcGenNames[] = {"","_MC_Pi0Merged","_MC_Pi0Decay","_MC_EtaDecay","_MC_PhotonOther","_MC_Electron","_MC_Other"};
    TString mcGenTitle[] = {"",",MC Pi0-Merged",",MC Pi0-Decay",", MC Eta-Decay",", MC Photon other sources",", MC Electron",", MC other sources"};
    for(Int_t igen = 0; igen < GetNCocktailGenNamesToCheck(); igen++)
    {
      TString add = "_MainGener_";
      if(igen==0) add = "";
      for(Int_t imc = 0; imc < fgkNGenTypes; imc++)
      {
        fhCleanGeneratorCluster[igen][imc] = new TH1F(Form("hCleanGeneratorCluster%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                      Form("Number of selected clusters with contribution of %s generator%s, no overlap",
                                                           GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                      nptbins,ptmin,ptmax);
        fhCleanGeneratorCluster[igen][imc]->SetYTitle("#it{counts}");
        fhCleanGeneratorCluster[igen][imc]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhCleanGeneratorCluster[igen][imc]) ;
        
        fhCleanGeneratorClusterEPrimRecoRatio[igen][imc] = new TH2F(Form("hCleanGeneratorClusterEPrimRecoRatio%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                    Form("#it{E}_{reco}/#it{E}_{gen} clusters with contribution of %s generator%s, no overlap",
                                                                         GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                    nptbins,ptmin,ptmax,nratbins,ratmin,ratmax);
        fhCleanGeneratorClusterEPrimRecoRatio[igen][imc]->SetYTitle("#it{E}_{reco}/#it{E}_{gen}");
        fhCleanGeneratorClusterEPrimRecoRatio[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhCleanGeneratorClusterEPrimRecoRatio[igen][imc]) ;
        
        fhCleanGeneratorClusterEPrimRecoDiff[igen][imc] = new TH2F(Form("hCleanGeneratorClusterEPrimRecoDiff%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                   Form("#it{E}_{reco}-#it{E}_{gen} clusters with contribution of %s generator%s, no overlap",
                                                                        GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                   nptbins,ptmin,ptmax,ndifbins,difmin,difmax);
        fhCleanGeneratorClusterEPrimRecoDiff[igen][imc]->SetYTitle("#it{E}_{reco}-#it{E}_{gen} (GeV)");
        fhCleanGeneratorClusterEPrimRecoDiff[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhCleanGeneratorClusterEPrimRecoDiff[igen][imc]) ;
     
        //
        
        fhMergeGeneratorCluster[igen][imc] = new TH1F(Form("hMergeGeneratorCluster%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                      Form("Number of selected clusters with contribution of >=2 generators, main %s%s",
                                                           GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                      nptbins,ptmin,ptmax);
        fhMergeGeneratorCluster[igen][imc]->SetYTitle("#it{counts}");
        fhMergeGeneratorCluster[igen][imc]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMergeGeneratorCluster[igen][imc]) ;
        
        fhMergeGeneratorClusterEPrimRecoRatio[igen][imc] = new TH2F(Form("hMergeGeneratorClusterEPrimRecoRatio%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                    Form("#it{E}_{reco}/#it{E}_{gen}clusters with contribution of >=2 generators, main %s%s",
                                                                         GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                    nptbins,ptmin,ptmax,nratbins,ratmin,ratmax);
        fhMergeGeneratorClusterEPrimRecoRatio[igen][imc]->SetYTitle("#it{E}_{reco}/#it{E}_{gen}");
        fhMergeGeneratorClusterEPrimRecoRatio[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterEPrimRecoRatio[igen][imc]) ;
        
        fhMergeGeneratorClusterEPrimRecoDiff[igen][imc] = new TH2F(Form("hMergeGeneratorClusterEPrimRecoDiff%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                   Form("#it{E}_{reco}-#it{E}_{gen}clusters with contribution of >=2 generators, main %s%s",
                                                                        GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                   nptbins,ptmin,ptmax,ndifbins,difmin,difmax);
        fhMergeGeneratorClusterEPrimRecoDiff[igen][imc]->SetYTitle("#it{E}_{reco}-#it{E}_{gen} (GeV)");
        fhMergeGeneratorClusterEPrimRecoDiff[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterEPrimRecoDiff[igen][imc]) ;
        
        //if(GetCocktailGenNameToCheck(igen).Contains("ijing")) continue;
        
        //
        
        fhMergeGeneratorClusterNotHijingBkg[igen][imc] = new TH1F(Form("hMergeGeneratorClusterNotHijingBkg%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                  Form("Number of selected clusters with contribution of >=2 generators, , none is HIJING, main %s%s",
                                                                       GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                  nptbins,ptmin,ptmax);
        fhMergeGeneratorClusterNotHijingBkg[igen][imc]->SetYTitle("#it{counts}");
        fhMergeGeneratorClusterNotHijingBkg[igen][imc]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterNotHijingBkg[igen][imc]) ;
        
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[igen][imc] = new TH2F(Form("hMergeGeneratorClusterNotHijingBkgEPrimRecoRatio%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                                Form("#it{E}_{reco}/#it{E}_{gen} clusters with contribution of >=2 generators, , none is HIJING, main %s%s",
                                                                                     GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                                nptbins,ptmin,ptmax,nratbins,ratmin,ratmax);
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[igen][imc]->SetYTitle("#it{E}_{reco}/#it{E}_{gen}");
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[igen][imc]) ;
        
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[igen][imc] = new TH2F(Form("hMergeGeneratorClusterNotHijingBkgEPrimRecoDiff%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                               Form("#it{E}_{reco}-#it{E}_{gen} clusters with contribution of >=2 generators, , none is HIJING, main %s%s",
                                                                                    GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                               nptbins,ptmin,ptmax,ndifbins,difmin,difmax);
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[igen][imc]->SetYTitle("#it{E}_{reco}-#it{E}_{gen} (GeV)");
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[igen][imc]) ;

        //
        
        fhMergeGeneratorClusterHijingAndOtherBkg[igen][imc] = new TH1F(Form("hMergeGeneratorClusterHijingAndOtherBkg%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                       Form("Number of selected clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                            GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                       nptbins,ptmin,ptmax);
        fhMergeGeneratorClusterHijingAndOtherBkg[igen][imc]->SetYTitle("#it{counts}");
        fhMergeGeneratorClusterHijingAndOtherBkg[igen][imc]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingAndOtherBkg[igen][imc]) ;
        
        
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[igen][imc] = new TH2F(Form("hMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                                     Form("#it{E}_{reco}/#it{E}_{gen} clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                                          GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                                     nptbins,ptmin,ptmax,nratbins,ratmin,ratmax);
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[igen][imc]->SetYTitle("#it{E}_{reco}/#it{E}_{gen}");
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[igen][imc]) ;
        
        
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[igen][imc] = new TH2F(Form("hMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                                    Form("#it{E}_{reco}-#it{E}_{gen} clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                                         GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                                    nptbins,ptmin,ptmax,ndifbins,difmin,difmax);
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[igen][imc]->SetYTitle("#it{E}_{reco}-#it{E}_{gen} (GeV)");
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[igen][imc]) ;

        //
        
        fhMergeGeneratorClusterHijingBkg[igen][imc] = new TH1F(Form("hMergeGeneratorClusterHijingBkg%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                               Form("Number of selected clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                    GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                               nptbins,ptmin,ptmax);
        fhMergeGeneratorClusterHijingBkg[igen][imc]->SetYTitle("#it{counts}");
        fhMergeGeneratorClusterHijingBkg[igen][imc]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingBkg[igen][imc]) ;
        
        fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[igen][imc] = new TH2F(Form("hMergeGeneratorClusterHijingBkgEPrimRecoRatio%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                             Form("#it{E}_{reco}/#it{E}_{gen} clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                                  GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                             nptbins,ptmin,ptmax,nratbins,ratmin,ratmax);
        fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[igen][imc]->SetYTitle("#it{E}_{reco}/#it{E}_{gen}");
        fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[igen][imc]) ;
       
        fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[igen][imc] = new TH2F(Form("hMergeGeneratorClusterHijingBkgEPrimRecoDiff%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                             Form("#it{E}_{reco}-#it{E}_{gen} clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                                  GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                             nptbins,ptmin,ptmax,ndifbins,difmin,difmax);
        fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[igen][imc]->SetYTitle("#it{E}_{reco}-#it{E}_{gen} (GeV)");
        fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[igen][imc]) ;
       }
    }
  }
  
  return outputContainer ;
}

//_______________________
/// Init. Do some checks, abort if the cluster is not the expected PHOS or EMCal
//_______________________
void AliAnaPhoton::Init()
{
  if      ( GetCalorimeter() == kPHOS  && !GetReader()->IsPHOSSwitchedOn()  && NewOutputAOD() )
    AliFatal("!!STOP: You want to use PHOS in analysis but it is not read!! \n!!Check the configuration file!!");
  else  if( GetCalorimeter() == kEMCAL && !GetReader()->IsEMCALSwitchedOn() && NewOutputAOD() )
    AliFatal("!!STOP: You want to use EMCAL in analysis but it is not read!! \n!!Check the configuration file!!");

  // Trick to select primary photon particles in the analysis of pure MC input
  if(GetReader()->GetDataType() == AliCaloTrackReader::kMC) GetCaloPID()->SwitchOnBayesian();
}

//_________________________________
/// Initialize the parameters of the analysis.
//_________________________________
void AliAnaPhoton::InitParameters()
{
  AddToHistogramsName("AnaPhoton_");
  
  fMinDist     = 2.;
  fMinDist2    = 4.;
  fMinDist3    = 5.;
	
  fTimeCutMin  =-1000000;
  fTimeCutMax  = 1000000;
  fNCellsCut   = 0;
	
  fRejectTrackMatch       = kTRUE ;
}

//_______________________________________
/// Do photon analysis and fill output particle aods with the selected clusters.
/// Clusters are selected in method ClustersSelected, and besides there
/// is a shower shape selection done after.
//_______________________________________
void  AliAnaPhoton::MakeAnalysisFillAOD()
{
  // Get the vertex
  Double_t v[3] = {0,0,0}; //vertex ;
  GetReader()->GetVertex(v);
  
  // Select the Calorimeter of the photon
  TObjArray * pl = 0x0;
  AliVCaloCells* cells    = 0;
  if      (GetCalorimeter() == kPHOS )
  {
    pl    = GetPHOSClusters();
    cells = GetPHOSCells();
  }
  else if (GetCalorimeter() == kEMCAL)
  {
    pl    = GetEMCALClusters();
    cells = GetEMCALCells();
  }
  
  if(!pl)
  {
    AliWarning(Form("TObjArray with %s clusters is NULL!",GetCalorimeterString().Data()));
    return;
  }
  
  TString genName = ""; 
  
  // Loop on raw clusters before filtering in the reader and fill control histogram
  if((GetReader()->GetEMCALClusterListName()=="" && GetCalorimeter()==kEMCAL) || GetCalorimeter()==kPHOS)
  {
    for(Int_t iclus = 0; iclus < GetReader()->GetInputEvent()->GetNumberOfCaloClusters(); iclus++ )
    {
      AliVCluster * clus = GetReader()->GetInputEvent()->GetCaloCluster(iclus);
        
      if     (GetCalorimeter() == kPHOS  && clus->IsPHOS()  && clus->E() > GetReader()->GetPHOSPtMin() )
      {
        clus->GetMomentum(fMomentum,GetVertex(0)) ;
          
        fhClusterCutsE [0]->Fill(fMomentum.E() , GetEventWeight());
        fhClusterCutsPt[0]->Fill(fMomentum.Pt(), GetEventWeight());
      }
      else if(GetCalorimeter() == kEMCAL && clus->IsEMCAL() && clus->E() > GetReader()->GetEMCALPtMin())
      {
        clus->GetMomentum(fMomentum,GetVertex(0)) ;
          
        fhClusterCutsE [0]->Fill(fMomentum.E(),  GetEventWeight());
        fhClusterCutsPt[0]->Fill(fMomentum.Pt(), GetEventWeight());
      }
    }
  }
  else
  { // reclusterized
    TClonesArray * clusterList = 0;
    
    if(GetReader()->GetInputEvent()->FindListObject(GetReader()->GetEMCALClusterListName()))
      clusterList = dynamic_cast<TClonesArray*> (GetReader()->GetInputEvent()->FindListObject(GetReader()->GetEMCALClusterListName()));
    else if(GetReader()->GetOutputEvent())
      clusterList = dynamic_cast<TClonesArray*> (GetReader()->GetOutputEvent()->FindListObject(GetReader()->GetEMCALClusterListName()));
    
    if(clusterList)
    {
      Int_t nclusters = clusterList->GetEntriesFast();
      for (Int_t iclus =  0; iclus <  nclusters; iclus++)
      {
        AliVCluster * clus = dynamic_cast<AliVCluster*> (clusterList->At(iclus));
          
        if(clus && clus->E() > GetReader()->GetEMCALPtMin())
        {
          clus->GetMomentum(fMomentum,GetVertex(0)) ;

          fhClusterCutsE [0]->Fill(clus->E()     , GetEventWeight());
          fhClusterCutsPt[0]->Fill(fMomentum.Pt(), GetEventWeight());
        }
      }
    }
  }
  
  // Init arrays, variables, get number of clusters
  Int_t nCaloClusters = pl->GetEntriesFast();
  
  AliDebug(1,Form("Input %s cluster entries %d", GetCalorimeterString().Data(), nCaloClusters));
  
  //----------------------------------------------------
  // Fill AOD with PHOS/EMCAL AliCaloTrackParticle objects
  //----------------------------------------------------
  // Loop on clusters
  for(Int_t icalo = 0; icalo < nCaloClusters; icalo++)
  {
    AliVCluster * calo =  (AliVCluster*) (pl->At(icalo));
    //printf("calo %d, %f\n",icalo,calo->E());
    
    //Get the index where the cluster comes, to retrieve the corresponding vertex
    Int_t evtIndex = 0 ;
    if (GetMixedEvent())
    {
      evtIndex=GetMixedEvent()->EventIndexForCaloCluster(calo->GetID()) ;
      //Get the vertex and check it is not too large in z
      if(TMath::Abs(GetVertex(evtIndex)[2])> GetZvertexCut()) continue;
    }
    
    //Cluster selection, not charged, with photon id and in fiducial cut
    if(GetReader()->GetDataType() != AliCaloTrackReader::kMC)
    {
      calo->GetMomentum(fMomentum,GetVertex(evtIndex)) ;
    }//Assume that come from vertex in straight line
    else
    {
      Double_t vertex[]={0,0,0};
      calo->GetMomentum(fMomentum,vertex) ;
    }
    
    //-------------------------------------
    // Play with the MC stack if available
    //-------------------------------------
    
    // Check origin of the candidates
    // recover different MC information
    Bool_t  ok = kFALSE;
    Int_t   pdg = 0, status = 0, momLabel = -1;
    Int_t   tag       = -1;
    Float_t mcbin     = -1;
    Int_t   noverlaps = -1;
    Float_t egen      =  0;
    Bool_t conversion = kFALSE; 

    const Int_t mcLabel = calo->GetLabel();
    const Int_t nlabels = calo->GetNLabels();
    Float_t ener        = calo->E();
    
    Int_t   index    = 0;
    Float_t weightPt = 1 ; 
    
    if ( IsDataMC() && mcLabel >= 0 )
    {
      if ( !GetMC() ) AliWarning("No MC pointer!");
      
      tag = GetMCAnalysisUtils()->CheckOrigin(calo->GetLabels(),
                                              calo->GetClusterMCEdepFraction(),
                                              nlabels, 
                                              GetMC(), 
                                              GetReader()->GetNameOfMCEventHederGeneratorToAccept(),
                                              fMomentum.E(), 
                                              pl); // check lost decays
      
      AliDebug(1,Form("Origin of candidate, bit map %d",tag));

      conversion = GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCConversion);

//        GetMCAnalysisUtils()->PrintMCTag(tag);
//        GetMCAnalysisUtils()->PrintAncestry(GetMC(),calo->GetLabel());

      fPrimaryMom = GetMCAnalysisUtils()->GetMother(mcLabel, GetMC(), pdg, status, ok, momLabel);     
      
      // In some histograms, each particle is a different bin
      if       ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0)       ){ mcbin = 1.5; pdg = 111;
        fPrimaryMom = GetMCAnalysisUtils()->GetMotherWithPDG(mcLabel, 111, GetMC(),ok, momLabel);        }
      else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEta)       ){ mcbin = 1.5; pdg = 221;
        fPrimaryMom = GetMCAnalysisUtils()->GetMotherWithPDG(mcLabel, 221, GetMC(),ok, momLabel);        }
      else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPhoton)     ) mcbin = 0.5;
      else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCElectron)   ) mcbin = 2.5;
      else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCMuon)       ) mcbin = 3.5;
      else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPion)       ) mcbin = 4.5;
      else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCKaon)       ) mcbin = 5.5;
      else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCProton)     ) mcbin = 6.5;
      else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCAntiProton) ) mcbin = 7.5;    
      else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCNeutron)    ) mcbin = 8.5;
      else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCAntiNeutron)) mcbin = 9.5;
      else                                                                                  mcbin = 10.5 ;
      
      /// Generated mother kine, ID and pT dependent Weight
      index    = GetReader()->GetCocktailGeneratorAndIndex(mcLabel, genName);
      
      if ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0)  ||
           GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0Decay) 
          )
      {
        fPrimaryMom2 = fPrimaryMom;
        if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0Decay))
          fPrimaryMom2 = GetMCAnalysisUtils()->GetMotherWithPDG(mcLabel, 111, GetMC(),ok, momLabel);        
        
        weightPt = GetParticlePtWeight(fPrimaryMom2.Pt(), 111, genName, index) ; 
      }
      
      else if ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEta)  ||
                GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEtaDecay) 
               )
      {
        fPrimaryMom2 = fPrimaryMom;
        if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEtaDecay))
          fPrimaryMom2 = GetMCAnalysisUtils()->GetMotherWithPDG(mcLabel, 221, GetMC(),ok, momLabel);        
        
        weightPt = GetParticlePtWeight(fPrimaryMom2.Pt(), 221, genName, index) ; 
      }
      else
      {
        weightPt = GetParticlePtWeight(fPrimaryMom.Pt(), pdg, genName, index) ; 
      }
      
      // Check if several particles contributed to cluster and discard overlapped mesons
      // Compare the primary depositing more energy with the rest,
      // if no photon/electron as comon ancestor (conversions), count as other particle
      Int_t overpdg[nlabels];
      Int_t overlab[nlabels];
      noverlaps = GetMCAnalysisUtils()->GetNOverlaps(calo->GetLabels(), nlabels,tag,-1,GetMC(),overpdg,overlab);
      
      // Fill first raw histograms
      fhMCParticle[0]->Fill(ener, mcbin, GetEventWeight()*weightPt);
      
      egen = fPrimaryMom.E();      
      if ( egen > 0.1 )
        fhMCParticleVsErecEgen[0]->Fill(ener, mcbin, ener/egen, GetEventWeight()*weightPt);
      
      if ( conversion )
        fhMCParticleConverted[0]->Fill(ener, mcbin, GetEventWeight()*weightPt);
      
      if ( fCheckOverlaps )
        fhMCParticleVsNOverlaps[0]->Fill(ener, mcbin, noverlaps, GetEventWeight()*weightPt);
    }
    
    //-----------------------------
    // Cluster selection
    //-----------------------------
    Int_t  nMaxima = GetCaloUtils()->GetNumberOfLocalMaxima(calo, cells); // NLM
    Int_t  nSM     = GetModuleNumber(calo);    
    Bool_t bRes = kFALSE, bEoP = kFALSE;
    Bool_t matched = IsTrackMatched(calo,GetReader()->GetInputEvent(),bEoP,bRes);

    if ( !ClusterSelected(calo, nSM, nMaxima, matched, bEoP, bRes,
                          tag, mcbin, egen, noverlaps, weightPt) ) continue;
    
    //----------------------------
    // Create AOD for analysis
    //----------------------------
    AliCaloTrackParticle aodph = AliCaloTrackParticle(fMomentum);
    
    //...............................................
    // Set the indeces of the original caloclusters (MC, ID), and calorimeter
    Int_t label = calo->GetLabel();
    aodph.SetLabel(label);
    aodph.SetWeight(weightPt);
    aodph.SetCaloLabel(calo->GetID(),-1);
    aodph.SetTag(tag);
    aodph.SetDetectorTag(GetCalorimeter());
    //printf("Index %d, Id %d, iaod %d\n",icalo, calo->GetID(),GetOutputAODBranch()->GetEntriesFast());
    
    //...............................................
    // Set bad channel distance bit
    Double_t distBad=calo->GetDistanceToBadChannel() ; //Distance to bad channel
    if     (distBad > fMinDist3) aodph.SetDistToBad(2) ;
    else if(distBad > fMinDist2) aodph.SetDistToBad(1) ;
    else                         aodph.SetDistToBad(0) ;
    //printf("DistBad %f Bit %d\n",distBad, aodph.DistToBad());
    
    //--------------------------------------------------------
    // Fill some shower shape histograms before PID is applied
    //--------------------------------------------------------
    
    Float_t maxCellFraction = 0;
    Int_t absIdMax = GetCaloUtils()->GetMaxEnergyCell(cells, calo, maxCellFraction);
    if( absIdMax < 0 ) AliFatal("Wrong absID");
    
    Int_t largeTimeInCellCluster = kFALSE;
    FillShowerShapeHistograms(calo, nSM, tag, weightPt, nMaxima, matched, 
                              maxCellFraction, largeTimeInCellCluster);
    
    aodph.SetFiducialArea(largeTimeInCellCluster); // Temporary use of this container, FIXME
    //if(largeTimeInCellCluster > 1) printf("Set n cells large time %d, pt %2.2f\n",aodph.GetFiducialArea(),aodph.Pt());
    
    aodph.SetM02(calo->GetM02());
    aodph.SetM20(calo->GetM20());
    aodph.SetNLM(nMaxima);
    
    Float_t time = calo->GetTOF()*1e9;
    if(time > 400) time-=fConstantTimeShift; // in case of clusterizer running before (calibrate clusters not cells)
    aodph.SetTime(time);
    
    aodph.SetNCells(calo->GetNCells());
    aodph.SetSModNumber(nSM);

    aodph.SetCellAbsIdMax(absIdMax);
    
    Float_t en  = fMomentum.E ();
    Float_t pt  = fMomentum.Pt();
    Float_t eta = fMomentum.Eta();
    Float_t phi = GetPhi(fMomentum.Phi());
    
    Int_t icolAbs = -1, irowAbs = -1;
    if ( fFillEBinAcceptanceHisto )
    {
      Float_t maxCellFraction = 0;
      Int_t absIdMax = GetCaloUtils()->GetMaxEnergyCell(cells,calo,maxCellFraction);
      
      Int_t icol = -1, irow = -1, iRCU = -1; 
      GetModuleNumberCellIndexesAbsCaloMap(absIdMax,GetCalorimeter(), icol, irow, iRCU, icolAbs, irowAbs);
      
      fhEnergyEtaPhi->Fill(en,eta,phi,GetEventWeight()*weightPt) ;
      
      fhEnergyColRow->Fill(en,icolAbs,irowAbs,GetEventWeight()*weightPt) ;
    }

    //-------------------------------------
    // PID selection or bit setting
    //-------------------------------------
    
    //...............................................
    // Data, PID check on
    if ( IsCaloPIDOn() )
    {
      // Get most probable PID, 2 options check bayesian PID weights or redo PID
      // By default, redo PID
      
      aodph.SetIdentifiedParticleType(GetCaloPID()->GetIdentifiedParticleType(calo));
      
      AliDebug(1,Form("PDG of identified particle %d",aodph.GetIdentifiedParticleType()));
      
      //If cluster does not pass pid, not photon, skip it.
      if ( aodph.GetIdentifiedParticleType() != AliCaloPID::kPhoton ) continue ;
    }
    
    //...............................................
    // Data, PID check off
    else
    {
      // Set PID bits for later selection (AliAnaPi0 for example)
      // GetIdentifiedParticleType already called in SetPIDBits.
      
      GetCaloPID()->SetPIDBits(calo,&aodph, GetCaloUtils(),GetReader()->GetInputEvent());
      
      AliDebug(1,"PID Bits set");
    }
    
    AliDebug(1,Form("Photon selection cuts passed: pT %3.2f, pdg %d",
                    aodph.Pt(),aodph.GetIdentifiedParticleType()));
    
    fhClusterCutsE [9]->Fill(en, GetEventWeight()*weightPt);
    fhClusterCutsPt[9]->Fill(pt, GetEventWeight()*weightPt);
    
    // Select only clusters with MC signal and data background
    //
    if ( SelectEmbededSignal() && IsDataMC() )
    {
      if ( nlabels == 0 || mcLabel < 0 ) continue;
      //else printf("Embedded cluster,  %d, n label %d label %d  \n",iclus,clus->GetNLabels(),clus->GetLabel());
      
      fhClusterCutsE [10]->Fill(en, GetEventWeight()*weightPt);
      fhClusterCutsPt[10]->Fill(pt, GetEventWeight()*weightPt);
    }
    
    if ( IsDataMC() )
    {
      // Fill final selected photon histograms in MC
      fhMCParticle[3]->Fill(ener, mcbin, GetEventWeight()*weightPt);
      
      if ( egen > 0.1 )
        fhMCParticleVsErecEgen[3]->Fill(ener, mcbin, ener/egen, GetEventWeight()*weightPt);
      
      if ( conversion  )
        fhMCParticleConverted[3]->Fill(ener, mcbin, GetEventWeight()*weightPt);
      
      if ( fCheckOverlaps )
        fhMCParticleVsNOverlaps[3]->Fill(ener, mcbin, noverlaps, GetEventWeight()*weightPt);
    }
    
    //
    // Check local cluster activity around the current cluster
    //
    if ( fStudyActivityNearCluster && en > 1.5 ) // 1.5 GeV cut used on Pb-Pb analysis
      ActivityNearCluster(icalo,en,eta,phi,tag,pl);

    //
    // Check if other generators contributed to the cluster
    //
    if ( IsDataMC() && IsStudyClusterOverlapsPerGeneratorOn() )
      CocktailGeneratorsClusterOverlaps(calo,tag);
    
    if ( fFillEBinAcceptanceHisto )
    {  
      fhEnergyEtaPhiPID->Fill(en, eta,phi,GetEventWeight()*weightPt) ;
      
      fhEnergyColRowPID->Fill(en, icolAbs,irowAbs,GetEventWeight()*weightPt) ;
    }
    
    if ( !IsHighMultiplicityAnalysisOn() )
    {
      if ( nSM < fNModules && nSM >=0 )
      {
        if ( !fFillOnlyPtHisto )
          fhEPhotonSM ->Fill(en, nSM, GetEventWeight()*weightPt);
        fhPtPhotonSM->Fill(pt, nSM, GetEventWeight()*weightPt);
      }
      
      // Few more control histograms for selected clusters
      fhNCellsE ->Fill(en, calo->GetNCells()  , GetEventWeight()*weightPt);
      fhTimePt  ->Fill(pt, time               , GetEventWeight()*weightPt);
      fhNLocMaxE->Fill(en, nMaxima            , GetEventWeight()*weightPt);
    }
    else
    {
      Float_t cen = GetEventCentrality();

      if ( nSM < fNModules && nSM >=0 )
        fhPtPhotonCentralitySM->Fill(pt, nSM, cen, GetEventWeight()*weightPt);
      
      // Few more control histograms for selected clusters
      fhNCellsCentralityE ->Fill(en, calo->GetNCells()  , cen, GetEventWeight()*weightPt);
      fhNLocMaxCentralityE->Fill(en, nMaxima            , cen, GetEventWeight()*weightPt);
    }
    
    if ( !fFillOnlySimpleSSHisto )
      fhMaxCellDiffClusterE->Fill(en, maxCellFraction, GetEventWeight()*weightPt);
    
    if ( cells )
    {
      for(Int_t icell = 0; icell <  calo->GetNCells(); icell++)
        fhCellsE->Fill(en, cells->GetCellAmplitude(calo->GetCellsAbsId()[icell]), GetEventWeight()*weightPt);
    }
    
    // Matching after cuts
    if ( fFillTMHisto && fFillTMHistoAfterCut )         
      FillTrackMatchingResidualHistograms(calo, 1, nSM, matched, bEoP, bRes, 
                                          tag, mcbin, egen, noverlaps, weightPt);
    
    // Fill histograms to undertand pile-up before other cuts applied
    // Remember to relax time cuts in the reader
    if ( IsPileUpAnalysisOn() ) FillPileUpHistograms(calo,cells, absIdMax);
    
    // Add AOD with photon object to aod branch
    AddAODParticle(aodph);
  }// loop
  
  AliDebug(1,Form("End fill AODs, with %d entries",GetOutputAODBranch()->GetEntriesFast()));
}

//______________________________________________
// Fill histograms with selected clusters/output AOD particles.
//______________________________________________
void  AliAnaPhoton::MakeAnalysisFillHistograms()
{
  // In case of simulated data, fill acceptance histograms
  if ( IsDataMC() && IsGeneratedParticlesAnalysisOn() ) 
    FillAcceptanceHistograms();

  // Get vertex
  Double_t v[3] = {0,0,0}; //vertex ;
  GetReader()->GetVertex(v);
  //fhVertex->Fill(v[0],v[1],v[2]);
  if(TMath::Abs(v[2]) > GetZvertexCut()) return ; // done elsewhere for Single Event analysis, but there for mixed event
  
  //----------------------------------
  // Loop on stored AOD photons
  Int_t naod = GetOutputAODBranch()->GetEntriesFast();
  AliDebug(1,Form("AOD branch entries %d", naod));
  
  Float_t cen = GetEventCentrality();
  // printf("++++++++++ GetEventCentrality() %f\n",cen);
  
  Float_t ep  = GetEventPlaneAngle();

  for(Int_t iaod = 0; iaod < naod ; iaod++)
  {
    AliCaloTrackParticle* ph =  (AliCaloTrackParticle*) (GetOutputAODBranch()->At(iaod));
    Int_t pdg = ph->GetIdentifiedParticleType();
    
    AliDebug(2,Form("PDG %d, MC TAG %d, Calorimeter <%d>",ph->GetIdentifiedParticleType(),ph->GetTag(), ph->GetDetectorTag())) ;
    
    // If PID used, fill histos with photons in Calorimeter GetCalorimeter()
    if(IsCaloPIDOn() && pdg != AliCaloPID::kPhoton) continue;
    
    if(((Int_t) ph->GetDetectorTag()) != GetCalorimeter()) continue;
    
    AliDebug(2,Form("ID Photon: pt %f, phi %f, eta %f", ph->Pt(),ph->Phi(),ph->Eta())) ;
    
    //................................
    //Fill photon histograms
    Float_t ptcluster  = ph->Pt();
    Float_t phicluster = ph->Phi();
    Float_t etacluster = ph->Eta();
    Float_t ecluster   = ph->E();
    Float_t weightPt   = ph->GetWeight();
    
    if ( IsHighMultiplicityAnalysisOn() )
    {
      fhPtCentralityPhoton ->Fill(ptcluster,cen, GetEventWeight()*weightPt) ;
      fhPtEventPlanePhoton ->Fill(ptcluster,ep , GetEventWeight()*weightPt) ;
    }
    else
    {
      fhEPhoton   ->Fill(ecluster , GetEventWeight()*weightPt);
      fhPtPhoton  ->Fill(ptcluster, GetEventWeight()*weightPt);
    } 
    fhPhiPhoton ->Fill(ptcluster, phicluster, GetEventWeight()*weightPt);
    fhEtaPhoton ->Fill(ptcluster, etacluster, GetEventWeight()*weightPt);
      
    // Fill event track multiplicity and sum pT histograms vs track pT
    // Calculated in the reader to be used everywhere, so not redone here.
    if ( fFillTrackMultHistograms )
    {
      for(Int_t icut = 0; icut < GetReader()->GetTrackMultiplicityNPtCut(); icut++)
      {
        fhPtPhotonNTracks    [icut]->Fill(ptcluster, GetReader()->GetTrackMultiplicity(icut), GetEventWeight()*weightPt) ;
        fhPtPhotonSumPtTracks[icut]->Fill(ptcluster, GetReader()->GetTrackSumPt       (icut), GetEventWeight()*weightPt) ;
      }
    }
    
    if ( !fFillEBinAcceptanceHisto )
    {
      if     (ecluster   > 0.5) fhEtaPhiPhoton  ->Fill(etacluster, phicluster, GetEventWeight()*weightPt);
      else if(GetMinPt() < 0.5) fhEtaPhi05Photon->Fill(etacluster, phicluster, GetEventWeight()*weightPt);
    }
  
//    Comment this part, not needed but in case to know how to do it in the future
//    // Get original cluster, to recover some information
//    AliVCaloCells* cells    = 0;
//    TObjArray * clusters    = 0;
//    if(GetCalorimeter() == kEMCAL)
//    {
//      cells    = GetEMCALCells();
//      clusters = GetEMCALClusters();
//    }
//    else
//    {
//      cells    = GetPHOSCells();
//      clusters = GetPHOSClusters();
//    }
//    
//    Int_t iclus = -1;
//    AliVCluster *cluster = FindCluster(clusters,ph->GetCaloLabel(0),iclus);
//    if(cluster)
    
    //.......................................
    // Play with the MC data if available
    if ( IsDataMC() )
    {
      //....................................................................
      // Access MC information in stack if requested, check that it exists.
      Int_t label = ph->GetLabel();
      
      if(label < 0)
      {
        AliDebug(1,Form("*** bad label ***:  label %d", label));
        continue;
      }
      
      Float_t eprim   = 0;
      Float_t ptprim  = 0;
      Bool_t ok = kFALSE;
      Int_t pdg = 0, status = 0, momLabel = -1;
      
      //fPrimaryMom = GetMCAnalysisUtils()->GetMother(label,GetReader(),ok);
      fPrimaryMom = GetMCAnalysisUtils()->GetMother(label,GetMC(), pdg, status, ok, momLabel);     
      
      if(ok)
      {
        eprim   = fPrimaryMom.Energy();
        ptprim  = fPrimaryMom.Pt();
      }
      
      Int_t tag =ph->GetTag();
      Int_t mcParticleTag = -1;
      if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPhoton) && fhMCPt[kmcPhoton])
      {
        if ( !fFillOnlyPtHisto )
        {
          fhMCE     [kmcPhoton] ->Fill(ecluster , GetEventWeight()*weightPt);
          fhMC2E    [kmcPhoton] ->Fill(ecluster , eprim , GetEventWeight()*weightPt);
          fhMCDeltaE[kmcPhoton] ->Fill(ecluster , eprim-ecluster  , GetEventWeight()*weightPt);
        }
        
        fhMCPt     [kmcPhoton] ->Fill(ptcluster, GetEventWeight()*weightPt);
        fhMC2Pt    [kmcPhoton] ->Fill(ptcluster, ptprim, GetEventWeight()*weightPt);
        fhMCDeltaPt[kmcPhoton] ->Fill(ptcluster, ptprim-ptcluster, GetEventWeight()*weightPt);
        
        fhMCPhi[kmcPhoton] ->Fill(ecluster,phicluster, GetEventWeight()*weightPt);
        fhMCEta[kmcPhoton] ->Fill(ecluster,etacluster, GetEventWeight()*weightPt);
      
        if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCConversion) &&
           fhMCPt[kmcConversion])
        {
          if ( !fFillOnlyPtHisto )
          {
            fhMCE     [kmcConversion] ->Fill(ecluster , GetEventWeight()*weightPt);
            fhMC2E    [kmcConversion] ->Fill(ecluster , eprim , GetEventWeight()*weightPt);
            fhMCDeltaE[kmcConversion] ->Fill(ecluster , eprim-ecluster  , GetEventWeight()*weightPt);
          }
          
          fhMCPt     [kmcConversion] ->Fill(ptcluster, GetEventWeight()*weightPt);
          fhMC2Pt    [kmcConversion] ->Fill(ptcluster, ptprim, GetEventWeight()*weightPt);
          fhMCDeltaPt[kmcConversion] ->Fill(ptcluster, ptprim-ptcluster, GetEventWeight()*weightPt);
          
          fhMCPhi[kmcConversion] ->Fill(ptcluster, phicluster, GetEventWeight()*weightPt);
          fhMCEta[kmcConversion] ->Fill(ptcluster, etacluster, GetEventWeight()*weightPt);
        
          Int_t pdgD = 0, statusD = 0, daugLabel = -1;
          Bool_t okD = kFALSE;
          
          //fMomentum = 
          GetMCAnalysisUtils()->GetDaughter(0,momLabel,GetMC(),pdgD, statusD, okD, daugLabel, fProdVertex);
          
          if ( okD )
          {
            Float_t prodR = TMath::Sqrt(fProdVertex.X()*fProdVertex.X()+fProdVertex.Y()*fProdVertex.Y());

            //printf("Conversion: mom pdg %d (stat %d), 1st daugher %d (stat %d), mom label %d, org label %d, daugh label %d, prodR %f\n",pdg,status, pdgD, statusD, 
            //       momLabel, label,daugLabel,prodR);

            if ( fFillConversionVertexHisto )
            {
              
              fhMCConversionVertex->Fill(ptcluster,prodR,GetEventWeight()*weightPt);
              
              if(GetCalorimeter() == kEMCAL && GetFirstSMCoveredByTRD() >= 0 && ph->GetSModNumber() >= GetFirstSMCoveredByTRD() )
                fhMCConversionVertexTRD->Fill(ptcluster,prodR,GetEventWeight()*weightPt);
            }
            
            if ( fFillSSHistograms )
            {
              Float_t m02 = ph->GetM02();
              Float_t m20 = ph->GetM20();
              
              // conversion vertex vs shower shape
              if(fFillConversionVertexHisto)
              {
                Int_t convR = -1;
                if      ( prodR < 75.  ) convR = 0;
                else if ( prodR < 275. ) convR = 1;
                else if ( prodR < 375. ) convR = 2;
                else if ( prodR < 400. ) convR = 3;
                else if ( prodR < 430. ) convR = 4;
                else                     convR = 5;
                
                if ( convR >= 0 )
                {
                  fhMCConversionLambda0Rcut[convR]->Fill(ptcluster,m02,GetEventWeight()*weightPt);
                  fhMCConversionLambda1Rcut[convR]->Fill(ptcluster,m20,GetEventWeight()*weightPt);
                  
                  if ( GetCalorimeter() == kEMCAL && GetFirstSMCoveredByTRD() >= 0 && ph->GetSModNumber() >= GetFirstSMCoveredByTRD() )
                  {
                    fhMCConversionLambda0RcutTRD[convR]->Fill(ptcluster,m02,GetEventWeight()*weightPt);
                    fhMCConversionLambda1RcutTRD[convR]->Fill(ptcluster,m20,GetEventWeight()*weightPt);
                  }
                  //                //
                  //                // EMCAL SM regions
                  //                //
                  //                if ( GetCalorimeter() == kEMCAL && fFillEMCALRegionSSHistograms )
                  //                {
                  //                  // Get original cluster, needed to feed the subregion selection method
                  //                  
                  //                  Int_t iclus = -1;
                  //                  AliVCluster *cluster = FindCluster(GetEMCALClusters(),ph->GetCaloLabel(0),iclus);
                  //
                  //                  Int_t etaRegion = -1, phiRegion = -1;
                  //                  
                  //                  if ( cluster ) GetCaloUtils()->GetEMCALSubregion(cluster,GetReader()->GetEMCALCells(),etaRegion,phiRegion);
                  //                  
                  //                  if( etaRegion >= 0 && etaRegion < 4 && phiRegion >=0 && phiRegion < 3 ) 
                  //                  {
                  //                    fhLam0EMCALRegionMCConvRcut[etaRegion][phiRegion][convR]->Fill(ptcluster,m02, GetEventWeight()*weightPt);
                  //                    
                  //                    if ( GetFirstSMCoveredByTRD() >= 0 && ph->GetSModNumber() >= GetFirstSMCoveredByTRD()  )
                  //                      fhLam0EMCALRegionTRDMCConvRcut[etaRegion][phiRegion][convR]->Fill(ptcluster, m02, GetEventWeight()*weightPt);
                  //                    
                  //                  } // region found
                  //                } // check region
                } // conv region
              } // check conv region
              
            } // fill Sh Sh histograms
          } // okD
        } // conversion
        
        if     ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPrompt) )
        {
          mcParticleTag = kmcPrompt;
        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCFragmentation) )
        {
          mcParticleTag = kmcFragmentation;
        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCISR) )
        {
          mcParticleTag = kmcISR;
        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0) )
        {
          mcParticleTag = kmcPi0;

        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEta) )
        {
          mcParticleTag = kmcEta;

        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0Decay) )
        {
          mcParticleTag = kmcPi0Decay;
        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEtaDecay) )
        {
          mcParticleTag = kmcEtaDecay;
        }
        else
        {
          mcParticleTag = kmcOtherDecay;
        }
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCAntiNeutron) )
      {
        mcParticleTag = kmcAntiNeutron;
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCAntiProton) )
      {
        mcParticleTag = kmcAntiProton;
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCNeutron) )
      {
        mcParticleTag = kmcNeutron;
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCProton) )
      {
        mcParticleTag = kmcProton;
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPion) )
      {
        mcParticleTag = kmcChPion;
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCElectron) )
      {
        mcParticleTag = kmcElectron;
      }
      else if( fhMCE[kmcOther] )
      {
        mcParticleTag = kmcOther;
        
        //		 printf(" AliAnaPhoton::MakeAnalysisFillHistograms() - Label %d, pT %2.3f Unknown, bits set: ",
        //					ph->GetLabel(),ph->Pt());
        //		  for(Int_t i = 0; i < 20; i++) {
        //			  if(GetMCAnalysisUtils()->CheckTagBit(tag,i)) printf(" %d, ",i);
        //		  }
        //		  printf("\n");
      }
      
      //printf("mcParticleTag %d\n",mcParticleTag);
      //if ( mcParticleTag >= 0) printf("\t %p \n",fhMCE[mcParticleTag]);
      
      if ( mcParticleTag >= 0 && fhMCPt[mcParticleTag] )
      {
        if ( !fFillOnlyPtHisto )
        {
          fhMCE      [mcParticleTag]->Fill(ecluster , GetEventWeight()*weightPt);
          fhMC2E     [mcParticleTag]->Fill(ecluster , eprim , GetEventWeight()*weightPt);
          fhMCDeltaE [mcParticleTag]->Fill(ecluster , eprim-ecluster  , GetEventWeight()*weightPt);
        }
        
        fhMCPt     [mcParticleTag]->Fill(ptcluster, GetEventWeight()*weightPt);
        fhMC2Pt    [mcParticleTag]->Fill(ptcluster, ptprim, GetEventWeight()*weightPt);
        fhMCDeltaPt[mcParticleTag]->Fill(ptcluster, ptprim-ptcluster, GetEventWeight()*weightPt);
        
        fhMCPhi    [mcParticleTag]->Fill(ecluster,  phicluster, GetEventWeight()*weightPt);
        fhMCEta    [mcParticleTag]->Fill(ecluster,  etacluster, GetEventWeight()*weightPt);
        
     
      }
    }// Histograms with MC
  }// aod loop
}

//__________________________________________________
/// Print some relevant parameters set for the analysis.
//__________________________________________________
void AliAnaPhoton::Print(const Option_t * opt) const
{
  if(! opt)
    return;
  
  printf("**** Print %s %s ****\n", GetName(), GetTitle() ) ;
  AliAnaCaloTrackCorrBaseClass::Print(" ");
  
  printf("Calorimeter            =     %s\n", GetCalorimeterString().Data()) ;
  printf("Min Distance to Bad Channel   = %2.1f\n",fMinDist);
  printf("Min Distance to Bad Channel 2 = %2.1f\n",fMinDist2);
  printf("Min Distance to Bad Channel 3 = %2.1f\n",fMinDist3);
  printf("Reject clusters with a track matched = %d\n",fRejectTrackMatch);
  printf("Fill TM histo %d; after cut %d, with pT track dependence %d\n",
         fFillTMHisto, fFillTMHistoAfterCut, fFillTMHistoTrackPt);
  printf("Time Cut: %3.1f < TOF  < %3.1f\n", fTimeCutMin, fTimeCutMax);
  printf("Time shift: shift = %3.1f\n", fConstantTimeShift);
  printf("Number of cells in cluster is  > %d \n", fNCellsCut);
  printf("Number of local maxima in cluster is  %d < NLM < %d \n", fNLMCutMin,fNLMCutMax);
  printf("Fill shower shape histograms %d, per SM %d, per EMCal region %d, only simple %d, per NLM %d, conversion separation %d\n",
         fFillSSHistograms, fFillSSPerSMHistograms, fFillEMCALRegionSSHistograms, 
         fFillOnlySimpleSSHisto, fFillSSNLocMaxHisto, fSeparateConvertedDistributions);
  printf("Fill histo: conv vertex %d, track mult %d, control cluster content %d \n",
         fFillConversionVertexHisto,fFillTrackMultHistograms, fFillControlClusterContentHisto);  
  printf("Local cluster activity switch %d  \n",fStudyActivityNearCluster);  
  printf("Number of MC histograms: origin %d primary %d  \n", fNOriginHistograms, fNPrimaryHistograms);  
  printf("Fill Ebin acceptance histogram %d  \n", fFillEBinAcceptanceHisto);  
  printf("Check MC overlaps %d  \n", fCheckOverlaps);  
  printf("Fill only pT histo %d  \n", fFillOnlyPtHisto);  
    
  printf("    \n") ;
}
