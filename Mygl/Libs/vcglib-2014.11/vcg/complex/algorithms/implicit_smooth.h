/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
#ifndef __VCG_IMPLICIT_SMOOTHER
#define __VCG_IMPLICIT_SMOOTHER

#include <eigenlib/Eigen/Sparse>
#include <vcg/complex/algorithms/mesh_to_matrix.h>
#include <vcg/complex/algorithms/update/quality.h>
#include <vcg/complex/algorithms/smooth.h>

#define PENALTY 10000

namespace vcg{


template <class MeshType>
class ImplicitSmoother
{
    typedef typename MeshType::FaceType FaceType;
    typedef typename MeshType::VertexType VertexType;
    typedef typename MeshType::CoordType CoordType;
    typedef typename MeshType::ScalarType ScalarType;
    typedef typename Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> MatrixXm;


public:

    struct FaceConstraint
    {
        int numF;
        std::vector<ScalarType > BarycentricW;
        CoordType TargetPos;

        FaceConstraint()
        {
            numF=-1;
        }

        FaceConstraint(int _numF,
                       const std::vector<ScalarType > &_BarycentricW,
                       const CoordType &_TargetPos)
        {
            numF=_numF;
            BarycentricW= std::vector<ScalarType > (_BarycentricW.begin(),_BarycentricW.end());
            TargetPos=_TargetPos;
        }
    };

    struct Parameter
    {
        //the amount of smoothness, useful only if we set the mass matrix
        ScalarType lambda;
        //the use of mass matrix to keep the mesh close to its original position
        //(weighted per area distributed on vertices)
        bool useMassMatrix;
        //this bool is used to fix the border vertices of the mesh or not
        bool fixBorder;
        //this bool is used to set if cotangent weight is used, this flag to false means uniform laplacian
        bool useCotWeight;
        //the set of fixed vertices
        std::vector<int> FixedV;
        //the set of faces for barycentric constraints
        std::vector<FaceConstraint> ConstrainedF;
        //the degree of laplacian
        int degree;

        Parameter()
        {
            degree=1;
            lambda=0.2;
            useMassMatrix=true;
            fixBorder=false;
            useCotWeight=false;
        }
    };

private:


    static void InitSparse(const std::vector<std::pair<int,int> > &Index,
                           const std::vector<ScalarType> &Values,
                           const size_t m,
                           const size_t n,
                           Eigen::SparseMatrix<ScalarType>& X)
    {
        assert(Index.size()==Values.size());

        std::vector<Eigen::Triplet<ScalarType> > IJV;
        IJV.reserve(Index.size());

        for(size_t i= 0;i<Index.size();i++)
        {
            int row=Index[i].first;
            int col=Index[i].second;
            ScalarType val=Values[i];

            assert(row<m);
            assert(col<n);

            IJV.push_back(Eigen::Triplet<ScalarType>(row,col,val));
        }
        X.resize(m,n);
        X.setFromTriplets(IJV.begin(),IJV.end());
    }


    static void CollectHardConstraints(MeshType &mesh,const Parameter &SParam,
                                       std::vector<std::pair<int,int> > &IndexC,
                                       std::vector<ScalarType> &WeightC)
    {
        std::vector<int> To_Fix;

        //collect fixed vert
        if (SParam.fixBorder)
        {
            //add penalization constra
            for (int i=0;i<mesh.vert.size();i++)
            {
                if (!mesh.vert[i].IsB())continue;
                To_Fix.push_back(i);
            }
        }
        //add additional fixed vertices constraint
        To_Fix.insert(To_Fix.end(),SParam.FixedV.begin(),SParam.FixedV.end());

        //sort and make them unique
        std::sort(To_Fix.begin(),To_Fix.end());
        typename std::vector<int>::iterator it= std::unique (To_Fix.begin(), To_Fix.end());
        To_Fix.resize( std::distance(To_Fix.begin(),it) );

        for (size_t i=0;i<To_Fix.size();i++)
        {
            for (int j=0;j<3;j++)
            {
                int IndexV=(To_Fix[i]*3)+j;
                IndexC.push_back(std::pair<int,int>(IndexV,IndexV));
                WeightC.push_back((ScalarType)PENALTY);
            }
        }
    }

    static void CollectBarycentricConstraints(MeshType &mesh,
                                              const Parameter &SParam,
                                              std::vector<std::pair<int,int> > &IndexC,
                                              std::vector<ScalarType> &WeightC,
                                              std::vector<int> &IndexRhs,
                                              std::vector<ScalarType> &ValueRhs)
    {
        int baseIndex=mesh.vert.size();
        for (size_t i=0;i<SParam.ConstrainedF.size();i++)
        {
            //get the index of the current constraint
            int IndexConstraint=baseIndex+i;

            //add one hard constraint
            int FaceN=SParam.ConstrainedF[i].numF;
            assert(FaceN>=0);
            assert(FaceN<mesh.face.size());
            assert(mesh.face[FaceN].VN()==SParam.ConstrainedF[i].BarycentricW.size());

            //then add all the weights to impose the constraint
            for (size_t j=0;j<mesh.face[FaceN].VN();j++)
            {
                //get the current weight
                ScalarType currW=SParam.ConstrainedF[i].BarycentricW[j];

                //get the index of the current vertex
                int FaceVert=vcg::tri::Index(mesh,mesh.face[FaceN].V(j));

                //then add the constraints componentwise
                for (int k=0;k<3;k++)
                {
                    //multiply times 3 per component
                    int IndexV=(FaceVert*3)+k;

                    //get the index of the current constraint
                    int ComponentConstraint=(IndexConstraint*3)+k;
                    IndexC.push_back(std::pair<int,int>(ComponentConstraint,IndexV));

                    WeightC.push_back(currW*PENALTY);

                    IndexC.push_back(std::pair<int,int>(IndexV,ComponentConstraint));
                    WeightC.push_back(currW*PENALTY);

                    //this to avoid the 1 on diagonal last entry of mass matrix
                    IndexC.push_back(std::pair<int,int>(ComponentConstraint,ComponentConstraint));
                    WeightC.push_back(-1);
                }
            }

            for (int j=0;j<3;j++)
            {
                //get the index of the current constraint
                int ComponentConstraint=(IndexConstraint*3)+j;

                //get per component value
                ScalarType ComponentV=SParam.ConstrainedF[i].TargetPos.V(j);

                //add the diagonal value
                IndexRhs.push_back(ComponentConstraint);
                ValueRhs.push_back(ComponentV*PENALTY);
            }

        }
    }

public:


    static void Compute(MeshType &mesh, Parameter &SParam)
    {
        //calculate the size of the system
        int matr_size=mesh.vert.size()+SParam.ConstrainedF.size();

        //the laplacian and the mass matrix
        Eigen::SparseMatrix<ScalarType> L,M,B;

        //initialize the mass matrix
        std::vector<std::pair<int,int> > IndexM;
        std::vector<ScalarType> ValuesM;

        //add the entries for mass matrix
        if (SParam.useMassMatrix) MeshToMatrix<MeshType>::MassMatrixEntry(mesh,IndexM,ValuesM);
        //then add entries for lagrange mult due to barycentric constraints
        for (int i=0;i<SParam.ConstrainedF.size();i++)
        {
            int baseIndex=(mesh.vert.size()+i)*3;
            for (int j=0;j<3;j++)
            {
                IndexM.push_back(std::pair<int,int>(baseIndex+j,baseIndex+j));
                ValuesM.push_back(1);
            }
        }
        //add the hard constraints
        CollectHardConstraints(mesh,SParam,IndexM,ValuesM);
        //initialize sparse mass matrix
        InitSparse(IndexM,ValuesM,matr_size*3,matr_size*3,M);

        //initialize the barycentric matrix
        std::vector<std::pair<int,int> > IndexB;
        std::vector<ScalarType> ValuesB;

        std::vector<int> IndexRhs;
        std::vector<ScalarType> ValuesRhs;

        //then also collect hard constraints
        CollectBarycentricConstraints(mesh,SParam,IndexB,ValuesB,IndexRhs,ValuesRhs);
        //initialize sparse constraint matrix
        InitSparse(IndexB,ValuesB,matr_size*3,matr_size*3,B);

        //get the entries for laplacian matrix
        std::vector<std::pair<int,int> > IndexL;
        std::vector<ScalarType> ValuesL;
        MeshToMatrix<MeshType>::GetLaplacianMatrix(mesh,IndexL,ValuesL,SParam.useCotWeight);

        //initialize sparse laplacian matrix
        InitSparse(IndexL,ValuesL,matr_size*3,matr_size*3,L);

        for (int i=0;i<(SParam.degree-1);i++)L=L*L;

        //then solve the system
        Eigen::SparseMatrix<ScalarType> S = (M + B + SParam.lambda*L);

        //SimplicialLDLT
        Eigen::SimplicialCholesky<Eigen::SparseMatrix<ScalarType > > solver(S);
        assert(solver.info() == Eigen::Success);

        MatrixXm V(matr_size*3,1);

        //set the first part of the matrix with vertex values
        for (size_t i=0;i<mesh.vert.size();i++)
        {
            int index=i*3;
            V(index,0)=mesh.vert[i].P().X();
            V(index+1,0)=mesh.vert[i].P().Y();
            V(index+2,0)=mesh.vert[i].P().Z();
        }

        //then set the second part by considering RHS gien by barycentric constraint
        for (size_t i=0;i<IndexRhs.size();i++)
        {
            int index=IndexRhs[i];
            ScalarType val=ValuesRhs[i];
            V(index,0)=val;
        }

        //solve the system
        V = solver.solve(M*V).eval();

        //then copy back values
        for (size_t i=0;i<mesh.vert.size();i++)
        {
            int index=i*3;
            mesh.vert[i].P().X()=V(index,0);
            mesh.vert[i].P().Y()=V(index+1,0);
            mesh.vert[i].P().Z()=V(index+2,0);
        }
    }
};

}//end namespace vcg

#endif
