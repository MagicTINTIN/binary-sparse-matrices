<!-- LTeX: language=en-FR -->
# Changelog

All notable changes to this project will be documented in this file. See [conventional commits](https://www.conventionalcommits.org/) for commit guidelines.

---
## [\<1.1.0\>](https://github.com/MagicTINTIN/binary-sparse-matrices/releases/v1.1.0) - 2025-08-29 <small><small><small>[(compare)](https://github.com/MagicTINTIN/binary-sparse-matrices/compare/v1.0.0..v1.1.0)</small></small></small>

### 🚀 Features

- spreadsheet printning <small><small>[(6f733a4)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/6f733a4c3986e7614f58d35c40d65e866e162294)</small></small>
- nonzero addings when adding dimension <small><small>[(7158a02)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/7158a0248bf94a66a36d9a8edc69576844f6a8f2)</small></small>
- non-zero row/columns <small><small>[(8ac673c)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/8ac673cbbd8865eb25d9bce4ff55ecb2bc835dff)</small></small>
- info without med stat of full lines <small><small>[(a386400)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a386400419a71505e70af04eebea7e774558e40b)</small></small>
- get blil row <small><small>[(50da73e)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/50da73e5d36e2251ae365e9194941c82c8cda39c)</small></small>
- bcsr getRow + fix const keyword <small><small>[(a6e10aa)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a6e10aa5acfc18ff416ddc7bbfbd8472450884d9)</small></small>
- get method for csr and lil matrices <small><small>[(a333e70)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a333e708371bcdfd0642ca59b9ab91ee78f29b55)</small></small>
- rework operatorOr |= -> now returns if there is a change <small><small>[(643360d)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/643360ddb322513f460290284a38adca4acfc2eb)</small></small>
- getStats <small><small>[(86a362c)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/86a362c80f04d3ac992d165059ea9071804a5817)</small></small>

### 🐛 Bug Fixes

- case where new col value is at the line added <small><small>[(0abfa6c)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/0abfa6c157855dde7a770c55f6dc5ac2654d84af)</small></small>
- const keyword on get mehods and isValueIn function <small><small>[(94bc381)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/94bc381a5da0fa81800aef72ef159c96bb344390)</small></small>
- ci build missing algorithm header <small><small>[(12e332a)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/12e332a5203bdc8fd8cf191332c28a9d8afdacfa)</small></small>

### 🚜 Refactor

- dense matrix printing <small><small>[(3875786)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/387578671ab67deb4c725dfcde7a6631caf20e9e)</small></small>

### ⚙️ Miscellaneous Chores

- solution 1 to access blil _rows directly <small><small>[(3606434)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/3606434c0d34c1c8f0f31678af6b653ce78b5e11)</small></small>
- improving access to internal structure to prevent modification <small><small>[(28e8fbd)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/28e8fbd963b114763e019e520e95e66bf2f4f412)</small></small>
- throwing exceptions <small><small>[(90325a2)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/90325a2213be1496ef7063fabc72ae1aa4022438)</small></small>
- slightly better perfs in blil transpose without conversion <small><small>[(518f58a)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/518f58adf619f78229e64391cb6abeb2db36ac91)</small></small>
- not clear imrovement but globally yes a bit <small><small>[(143f5c3)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/143f5c3bd213eab6024b1589489fe4a833e1974c)</small></small>
- comply with std c++20 + homogen style of c++ <small><small>[(f29e4ab)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/f29e4aba858fdd92a7e25cebda152407082f5924)</small></small>

### 📚 Documentation

- cleaning, feature list <small><small>[(ae25dd6)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/ae25dd6a2b52258a1ac470748bd3fc89f7e11c44)</small></small>

### 🎨 Style

- improving spreadsheet readability using colors <small><small>[(fdd12f0)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/fdd12f0701db6d3a5c79c1fdedc507c538ddc571)</small></small>

### 🏗️ Build

- using clang to check implementation <small><small>[(6beaa6e)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/6beaa6e773a765d0a778e2e92d7faf9b489d2cb9)</small></small>

---
## [\<1.0.0\>](https://github.com/MagicTINTIN/binary-sparse-matrices/releases/v1.0.0) - 2025-06-25 <small><small><small>[(compare)](https://github.com/MagicTINTIN/binary-sparse-matrices/compare/v0.1.0..v1.0.0)</small></small></small>

### 🚀 Features

- adding BLIL structure <small><small>[(4cb5ecb)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/4cb5ecbfc9965c30bdcf3d6cb24ddadc4ce023c8)</small></small>
- implementing blil check order <small><small>[(b9b0195)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/b9b01953f49b384eaf40cceea3690cd55cf6f2b1)</small></small>
- csr to lil and lil to csr conversion <small><small>[(ad61901)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/ad61901a6844ec2e8ed2eb4899e36fd7c8854d3d)</small></small>
- conversion from/to blil/dense <small><small>[(921265e)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/921265edb0c058cb618e07161e41ecdd72cbd7b0)</small></small>
- blil toString representation <small><small>[(b255fbd)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/b255fbd22cfa64c4935b84d8e05cef2c0684bf7f)</small></small>
- set/reset lil value <small><small>[(c994d5b)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/c994d5b6c7d3e8f1fd528827542f027a0e1ae106)</small></small>
- insertByValue util (why templates... ;-;) <small><small>[(fb5ed33)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/fb5ed332d93f2bfe68695fb9efc1446dc233a3fc)</small></small>
- alternate set much faster with dichotomy <small><small>[(fac7da6)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/fac7da682ffb1cd80688760200319d9658a84d1f)</small></small>
- lil transpose (not tested yet) <small><small>[(7c899c0)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/7c899c09cf952dc5c99676ffee25f854a3009120)</small></small>
- dichotomic set/reset in blil too <small><small>[(7d97a27)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/7d97a27a0df84cedcb0caaa254a32b7ab633f8a9)</small></small>
- lil transpose opti by csr conversion <small><small>[(f8ed71b)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/f8ed71bb89b66c0b955608f77c92153a49265146)</small></small>
- LIL operation OR <small><small>[(3493642)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/34936423e3e6489862a268221a2e838f8eb438ac)</small></small>
- blil operator& + opti bcsr operationAnd with .erase(b,e) <small><small>[(06a7ca1)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/06a7ca11249c0926d4f877d4d756184fb1911700)</small></small>
- operator== to easily compare matrices (finally) <small><small>[(85136fc)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/85136fcffbc38bce5a753ef8a31028012451647b)</small></small>
- blil matmat multiplication implementation <small><small>[(c58c1a1)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/c58c1a1739ebf05b79bc5f648a452efe7cad4fdc)</small></small>
- 0 dim matrix initialisation <small><small>[(799f586)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/799f586dc8f69c7f21bd1c1cff96e54ad831200e)</small></small>
- add dimensions to sparse matrices <small><small>[(f4bdd1a)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/f4bdd1a7f7dd7861f76598b52921ce9a648e97ff)</small></small>
- csr matrix generator <small><small>[(2bdf8ad)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/2bdf8adf0f219758de8c594fe00ef19892887077)</small></small>
- info() method to easily print stats without all values <small><small>[(57504cd)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/57504cdaea62b58cfea7db950fe60e030d90b264)</small></small>

### 🐛 Bug Fixes

- removing circular dependency + accessing mutual private params <small><small>[(a9c2fa5)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a9c2fa56db11f40d81a4819bc2846f56410103c7)</small></small>
- toString information line mean -> min,med,max + lil toCondensedString with {} <small><small>[(47cb1c2)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/47cb1c24d4e4c6abb327c6117e04b057f67df7a8)</small></small>
- bcsrAnd not removing enough 1's in line post removing <small><small>[(a93c9bc)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a93c9bc11db69c482338796c4bbb51cd99f463a3)</small></small>
- function name mistake <small><small>[(1daeb3f)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/1daeb3f692fa39a8805c9e3fdea81acf37c01422)</small></small>
- BCSR addDimension multiple def of BLIL <small><small>[(2151f09)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/2151f097df7030990f71284fda9d9fd61803d778)</small></small>

### ⚙️ Miscellaneous Chores

- preparing lil operations, cleaning unused csr commented code, importing redundant methods from csr <small><small>[(9abb0ca)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/9abb0ca84f536d92ef8f88ff33ee2549b06cf50c)</small></small>
- removing less efficient methods <small><small>[(36e3c05)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/36e3c05d077b0e96d55ea11438ec72dadca9d8eb)</small></small>
- CSR now uses LIL for a quicker OR <small><small>[(533fb75)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/533fb755d8df982c18a7a0f9fd6b561d437e4a63)</small></small>
- bcsr & operator optimisation with conversion to blil <small><small>[(ef8d410)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/ef8d410eea4a21d201b5becbf56169531a8372f1)</small></small>
- attempt to improve bcsr matmat with sorted insertion : fail <small><small>[(9b48141)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/9b48141d4d30950ee30909609f720cd0a1455c18)</small></small>
- improving bcrs matmat by sorting at the end of each line <small><small>[(0fa7063)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/0fa70634b64bb85e93aec1dff552c6eead78bba1)</small></small>
- using the new operator* with sort opti <small><small>[(a62ca9d)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a62ca9d24596906cc8d817a676ede62c858383c3)</small></small>
- improved csr generator in c++ for really big matrices <small><small>[(5e1fd4e)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/5e1fd4ece32fde529dc8e35b3cf5d8ca6ccb6538)</small></small>

### 🧪 Tests

- csr>lil|>csr quicker than csr| <small><small>[(c975d44)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/c975d44c6f770de294d7577fce00f79a4a166038)</small></small>
- matmat multiplication, conversion to BLIL is not worth <small><small>[(a963fae)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a963faee04b5b4360f4050ad94ad7c81c57e50b0)</small></small>
- multiplication with 100k×100k matrix <small><small>[(d013006)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/d0130063ff30ace646dcf6e7ed86f83ebd8bdf57)</small></small>

---
## [\<0.1.0\>](https://github.com/MagicTINTIN/binary-sparse-matrices/releases/v0.1.0) - 2025-06-17

### 🚀 Features

- initialising bcsr <small><small>[(db311a2)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/db311a212db74a530674514da8e4cef6dc957fb6)</small></small>
- csr2dn + print <small><small>[(0d979a6)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/0d979a6c0a374e800ce7a1c7d1ad6b3380131ca1)</small></small>
- adding |= += constructors + doc <small><small>[(2f789b0)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/2f789b040eb78034121e5dce96e1f16b31f124ba)</small></small>
- set method <small><small>[(f7a4305)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/f7a43052e2a07c75fde05b1110ce05c0a1140f8b)</small></small>
- reset method <small><small>[(c11e566)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/c11e5660f51b15b4c30f20e4b47f47a5ca94401c)</small></small>
- array ordering checker on bcsr <small><small>[(cf12dd9)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/cf12dd97d5a05044724c4522061d4629e8934b80)</small></small>
- verbose ordering checker <small><small>[(e5ace2c)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/e5ace2ca2af43117d980829bbc7a449cc023cbc8)</small></small>
- nz number value <small><small>[(a8d69f1)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a8d69f1bee542b69b4ab0d965ea4d79eaace0fc9)</small></small>
- bcsr matrix transposition <small><small>[(9f6662d)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/9f6662d7c507220ba372e3c353d465a984fb5d8d)</small></small>
- adding bcsr operators + &= & definition <small><small>[(bb2b06d)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/bb2b06d4a27f8afe65654abf75de513277270f0c)</small></small>
- optimised csr &, &= opAnd works <small><small>[(42de842)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/42de842ab3c02d47131a58e2f1e165411e312b47)</small></small>
- matmat mulitplication <small><small>[(70be4cc)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/70be4cc19f32a2db5a888a54b398421f542026ff)</small></small>
- test zone & chrono <small><small>[(aedd685)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/aedd685609a7175cc380ba896b65c60b561dc06a)</small></small>
- matrix generator <small><small>[(a26340e)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a26340e77da8e8232b1b52644739617add51c103)</small></small>
- build debug and release each time <small><small>[(1f1d1b8)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/1f1d1b83d8ece4511721a3985e6df122dd0a9714)</small></small>
- function to make csr canonical <small><small>[(9c4472e)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/9c4472eadeb82e717ef05a4566cbb4a5ef6dc7e3)</small></small>
- multiplication algorithm <small><small>[(5d851ad)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/5d851ad439c8c3df01316f6fcbca79d89a155c0e)</small></small>

### 🐛 Bug Fixes

- switching to array like matrix representation for csr->dn <small><small>[(a1da09d)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a1da09d50be31e28fa195308b76198492b1ed3f6)</small></small>
- reset removing more than one value in the line <small><small>[(0bf8265)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/0bf8265e881e13c0d99daeee74e6cad52c5f787d)</small></small>
- ordering check signed/unsigned comparison <small><small>[(d3bacd8)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/d3bacd8528564eb2ea5bf7898baf01c0c0f3c4f4)</small></small>
- optimized operationOr, no more lineCarry <small><small>[(d06613f)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/d06613f8b610d4c4fb59c4df45d83aa819b3a910)</small></small>
- useless nz attribute <small><small>[(65f6169)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/65f616948ac6c06706f46922514fbd87256df47c)</small></small>
- display (update with _indices.size()) <small><small>[(4c6f6a1)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/4c6f6a10f21726ff93af44bb00351c4b3e6061a8)</small></small>
- bcsr transpose due to u8 instead of u32 <small><small>[(e342f60)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/e342f60969f69ab5f3de02f59ca496c20179b5ec)</small></small>
- init _indicies values, only reserving + time calculation <small><small>[(859d64b)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/859d64b96a5da9e694a4ff5b75f27bbca0c01f7c)</small></small>

### 🚜 Refactor

- using operator| in |= <small><small>[(4179c84)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/4179c848bf561e109e9e540bfd7957042277c71b)</small></small>
- moving conversion to a separate file <small><small>[(80d9378)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/80d93780d57b50fb188fb3335649189f2a9f95e6)</small></small>

### ⚙️ Miscellaneous Chores

- updating operator| performances with new Or implementation <small><small>[(acdfcf4)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/acdfcf4bb1b2087b13f921890e8bb9a82547e6ec)</small></small>
- opti no more useless length check <small><small>[(61b34f8)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/61b34f85a2e900c6f2a29040be628cfddd102057)</small></small>
- optimisation, not needing to check the full line <small><small>[(c58e181)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/c58e181d8b274e7d2f00eca44fa705e61f7d35d9)</small></small>
- trying to have a modified version of scipy with my modifications <small><small>[(49c6256)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/49c6256feb2299ef9542dd5518293fb008bc7c6c)</small></small>
- scipy's apparently uglier algorithm is in fact more efficient <small><small>[(851faf0)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/851faf00e60727bf3c898046c5cb73ae31e194b2)</small></small>
- adding some matrix examples <small><small>[(ea72969)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/ea72969b18ea992637f89ef12d3d54de2efc2d0c)</small></small>
- small timesMatrix improvement <small><small>[(044194b)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/044194b14ba782dc5daab2c53c5bed9ddb908a73)</small></small>
- improving bcsr scipy mult <small><small>[(0280760)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/0280760a480b44ef06b6ea9cb2279b566b384464)</small></small>
- small mat mat mult optimisation <small><small>[(fbdcad0)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/fbdcad084f4e8b36d656fe01b3a1d353875bbd9b)</small></small>
- perf improvement on operator* <small><small>[(a679d21)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/a679d21fb534c3091a695d0ee53f534a3c8e121b)</small></small>
- using new method on all operator* derivations <small><small>[(5238f97)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/5238f978b0f95d726812a57966f3a6c97bebdd07)</small></small>

### 📚 Documentation

- test results <small><small>[(bea4eaa)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/bea4eaadf441a063315089b801b68e4b4b5fca6a)</small></small>

### 🧪 Tests

- adding scipy reference <small><small>[(5af1cb7)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/5af1cb76db07511e2591394b95e7cdc612ba56ed)</small></small>
- comparison with scipy's methods <small><small>[(cad3930)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/cad3930eb9f30231bd49e03e35a1eba038f11d8a)</small></small>
- testing with big matrices <small><small>[(2f392ec)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/2f392ec6aaae957943fdbd08611d31c0d4d8c4f0)</small></small>
- more tests <small><small>[(fdf5caf)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/fdf5caf19a3e9bff5acd63f986793dfa3fcf5991)</small></small>
- bigger matrix <small><small>[(603ec66)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/603ec66c89631f941195e791678a4db2673ba828)</small></small>
- so the performance diff comes from the stack/heap allocation <small><small>[(9696f93)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/9696f93902154de1654220e31779c5ee6dd53ce7)</small></small>
- multiplication comparison with scipy <small><small>[(375616f)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/375616f707d644b22b7a3884fe534af23c73bbed)</small></small>
- adding metrics to understand the problem of timesMatrix <small><small>[(cd32b40)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/cd32b406984fde2627fbedffa6f1023694632143)</small></small>

### 🏗️ Build

- adding script to easily build <small><small>[(0471e72)](https://github.com/MagicTINTIN/binary-sparse-matrices/commit/0471e727ae4cc3fb7036a35e951a483ac8635c4b)</small></small>

<!-- generated by git-cliff -->
