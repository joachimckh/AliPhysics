#ifndef AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency_H
#define AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency_H

class TList;
class TH1F;
class TH1D;
class TH2D;
class TH3D;
class TF1;
class AliPIDResponse;

#include "AliAnalysisTaskSE.h"
#include "AliPIDResponse.h"
#include "AliEventCuts.h"
class AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency : public AliAnalysisTaskSE
{
public:
  AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency();
  AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency(const char *name);
  virtual ~AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency();

  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *option);

  //Analysis setters
  virtual void SetFilterBit(UInt_t Bit)
  {
    filterBit = Bit;
  }
  virtual void SetNumbersOfBins(int f_nPhiBins, int f_nVertexBins, int f_nPBins)
  {
    nPhiBins = f_nPhiBins;
    nVertexBins = f_nVertexBins;
    nPBins = f_nPBins;
  }
  virtual void SetCentLim(int f_minCent, int f_maxCent)
  {
    minCent = f_minCent;
    maxCent = f_maxCent;
  }
  virtual void SetPLim(Float_t f_minP, Float_t f_maxP)
  {
    minP = f_minP;
    maxP = f_maxP;
  }
  virtual void SetVertLim(Float_t f_Vertexmin, Float_t f_Vertexmax)
  {
    Vertexmin = f_Vertexmin;
    Vertexmax = f_Vertexmax;
  }
  virtual void SetParams(Bool_t f_IsMC, Bool_t f_pbpb, int f_nSigma, int f_nCrossedRows, int f_movePhi)
  {
    IsMC = f_IsMC;
    pbpb = f_pbpb; // pp if false
    nSigma = f_nSigma;
    nCrossedRows = f_nCrossedRows;
    movePhi = f_movePhi;
  }

private:
  AliAODEvent *fAOD;            //! input event
  AliPIDResponse *fPIDResponse; //! pid response object
  TList *fOutputList;           //! output list
  TH1F *fHistEventsCut;         //!
  TH1F *fHistTracksCut;         //!
  UInt_t filterBit;             //
  Bool_t pbpb, IsMC;
  AliEventCuts *fAliEventCuts; //!
  int nPhiBins, nVertexBins, nPBins, minCent, maxCent, nSigma, nCrossedRows, movePhi;
  Float_t minP, maxP, Vertexmin, Vertexmax;
  Float_t PtCut[3] = {0.2, 0.5, 0.5};
  Float_t nSigmaBoundary[3] = {0.5, 0.32, 0.7};

  //Efficiency Maps
  //Tracking:
  TH3D *NGenTracks[4][16][8];            //!
  TH3D *EfficiencyTracking[4][16][8];    //!
  TH3D *ContaminationTracking[4][16][8]; //!
  TH3D *NReconstTracks[4][16][8];        //!
  //PID:
  TH3D *NTrueTracks[4][16][8];      //!
  TH3D *EfficiencyPID[4][16][8];    //!
  TH3D *ContaminationPID[4][16][8]; //!
  TH3D *NTracksInCut[4][16][8];     //!

  TH2D *fDeDx;         //!
  TH2D *fDeDxSorts[4]; //!
  TH2D *fTOF;          //!
  TH2D *fTOFSorts[4];  //!

  TH1D *purityAll[8];      //!
  TH1D *purity[8][8]; //!

  TH2D *fHistQASPDTrackletsvsV0MCent; //!

  AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency(const AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency &);            // not implemented
  AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency &operator=(const AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency &); // not implemented

  ClassDef(AliAnalysisTaskParticleYieldRatioCorrelationsEfficiency, 1);
};

#endif
