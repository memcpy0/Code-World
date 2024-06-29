
> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。

我在配置Haskell语言学习环境时了解到了 `Haskell Language Server` 概念，这里就仔细探究一下，究竟什么是 `Language Server` 和 `Language Server Protocol` ，反正肯定不会是群众喜·闻·乐·见的L·S·P（ε=ε=ε=┏(゜ロ゜;)┛
![在这里插入图片描述](https://img-blog.csdnimg.cn/fd9731deb2ca4c0eaed38d46cbcd4abe.gif#pic_center)

---
# 1. 什么是LSP
引用[官网的定义](https://microsoft.github.io/language-server-protocol/)：
> The Language Server Protocol (LSP) defines the protocol used between an editor or IDE and a language server that provides language features like auto complete, go to definition, find all references etc. The goal of the Language Server Index Format (LSIF, pronounced like "else if") is to support rich code navigation in development tools or a Web UI without needing a local copy of the source code. 

即，语言服务器（`Language Server Protocl, LSP`）定义了一个在**编辑器或IDE**与**语言服务器**之间使用的协议，该语言服务器提供了诸如自动补全、跳转至定义、查找引用等语言功能。语言服务器索引格式（`Language Server Index Fromat, LSIF`，发音为 `else if` ）的目标是在开发工具或Web UI中支持丰富的代码导航，无需源代码的本地副本。

这无疑是一个伟大的变革！一般而言，想要在编辑器或IDE中为编程语言提供支持需要付出巨大的努力，**因为有多少编辑器或IDE就要有多少遍重复的工作**——毕竟每个开发工具提供的API是不同的，即使是某个相同的功能，也要根据不同的编辑器或IDE重复用不同的代码实现。比如想要在Vim、Emacs、VS Code三种编辑器中分别为C、C++、Java、Python四种语言提供支持，就要做 $3 \times 4 = 12$ 遍这种工作。语言服务器协议大大减少了这样无意义的工作量，从 $n \times m$ 量级降低到了 $n + m$ ，就类似于中间代码对编译器前后端实现的变革。



---
# Overview
https://microsoft.github.io/language-server-protocol/overviews/lsp/overview
The protocol defines the format of the messages sent using JSON-RPC between the development tool and the language server. LSIF defines a graph format to store information about programming artifacts.
Specification
https://microsoft.github.io/language-server-protocol/specifications/specification-current
The latest version of the LSP specification is version 3.16. There is now also a specification for the upcoming language server index format (LSIF).
Implementations
https://microsoft.github.io/language-server-protocol/implementors/servers/
The LSP has been implemented for many languages and many development tools are integrating these language servers.



超级吊！！！因为 lsp 的存在，所以 VSCode 只是一个遵循了 lsp 的 client 而已。既然只是个 client，以后 VSCode 的牛逼的功能（接近于 IDE 的静态分析等）几乎都能在 Emacs / Vim 上实现了，编辑器界终于达到了前所未有的大和谐，喜大普奔！emacs-lsp/lsp-mode
 https://link.zhihu.com/?target=https%3A//github.com/emacs-lsp/lsp-mode



<table class="table table-bordered table-responsive">
  <thead>
    <tr>
      <th>Language</th>
      <th>Maintainer</th>
      <th>Repository</th>
      <th>Implementation Language</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>1C Enterprise</td>
      <td><a href="https://github.com/1c-syntax/bsl-language-server/graphs/contributors">Contributors</a></td>
      <td><a href="https://github.com/1c-syntax/bsl-language-server">BSL Language Server</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td>ActionScript 2.0</td>
      <td><a href="https://github.com/admvx">Adam Vernon</a></td>
      <td><a href="https://github.com/admvx/as2-language-support">AS2 Language Support</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>Ada/SPARK</td>
      <td>AdaCore</td>
      <td><a href="https://github.com/AdaCore/ada_language_server">ada_language_server</a></td>
      <td>Ada</td>
    </tr>
    <tr>
      <td><a href="https://a.ml/">AML</a></td>
      <td><a href="https://a.ml/">AML</a></td>
      <td><a href="https://github.com/aml-org/als">AML Language Server</a></td>
      <td>ScalaJS</td>
    </tr>
    <tr>
      <td>Antlr</td>
      <td><a href="https://github.com/kaby76">Ken Domino</a></td>
      <td><a href="https://github.com/kaby76/AntlrVSIX">Antlrvsix</a></td>
      <td>C#</td>
    </tr>
    <tr>
      <td><a href="http://api-elements.readthedocs.io/en/latest/">API Elements</a></td>
      <td><a href="https://github.com/XVincentX">Vincenzo Chianese(@XVincentX)</a></td>
      <td><a href="https://github.com/XVincentX/vscode-apielements">vscode-apielements</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>Apache Camel</td>
      <td><a href="https://github.com/camel-tooling/camel-language-server/graphs/contributors">Contributors</a></td>
      <td><a href="https://github.com/camel-tooling/camel-language-server">Apache Camel Language Server</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td>Apex</td>
      <td>Salesforce</td>
      <td><a href="https://marketplace.visualstudio.com/items?itemName=salesforce.salesforcedx-vscode-apex">VS Code Apex extension</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>IBM High Level <strong>Assembler</strong></td>
      <td><a href="https://www.broadcom.com/">Broadcom</a></td>
      <td><a href="https://github.com/eclipse/che-che4z-lsp-for-hlasm">HLASM Language Support</a></td>
      <td>C++</td>
    </tr>
    <tr>
      <td><a href="https://www.ibm.com/support/knowledgecenter/SSLTBW_2.2.0/com.ibm.zos.v2r2.e0zc100/hlasm.htm">IBM High Level <strong>Assembler</strong></a></td>
      <td>IBM</td>
      <td><a href="https://marketplace.visualstudio.com/items?itemName=IBM.zopeneditor">IBM Z Open Editor VS Code extension</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td><a href="https://www.asyncapi.com/">AsyncAPI</a></td>
      <td><a href="https://a.ml/">AML</a></td>
      <td><a href="https://github.com/aml-org/als">AML Language Server</a></td>
      <td>ScalaJS</td>
    </tr>
    <tr>
      <td><a href="https://ballerina.io/">Ballerina</a></td>
      <td><a href="https://ballerina.io/">Ballerina.io</a></td>
      <td><a href="https://github.com/ballerina-platform/ballerina-lang/tree/master/language-server">Ballerina Language Server</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td>Bash</td>
      <td><a href="https://github.com/mads-hartmann/">Mads Hartmann</a></td>
      <td><a href="https://github.com/mads-hartmann/bash-language-server">Bash Language Server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="https://github.com/azure/bicep">Bicep</a></td>
      <td>MS</td>
      <td><a href="https://github.com/azure/bicep">Bicep</a></td>
      <td>C#</td>
    </tr>
    <tr>
      <td>BrightScript/BrighterScript</td>
      <td><a href="https://github.com/RokuCommunity">RokuCommunity</a></td>
      <td><a href="https://github.com/rokucommunity/brighterscript">brighterscript</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>C#</td>
      <td><a href="http://www.omnisharp.net/">OmniSharp</a></td>
      <td><a href="https://github.com/OmniSharp/csharp-language-server-protocol">csharp-language-server-protocol</a></td>
      <td>C#</td>
    </tr>
    <tr>
      <td>C++</td>
      <td>MS</td>
      <td><a href="https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools">VS Code C++ extension</a></td>
      <td>C++</td>
    </tr>
    <tr>
      <td><a href="https://clangd.llvm.org/">C++/clang</a></td>
      <td><a href="https://llvm.org/">LLVM Project</a></td>
      <td><a href="https://github.com/llvm/llvm-project/tree/main/clang-tools-extra/clangd">clangd</a></td>
      <td>C++</td>
    </tr>
    <tr>
      <td>C/C++/Objective-C</td>
      <td><a href="https://github.com/jacobdufault">Jacob Dufault</a>, <a href="https://github.com/MaskRay">MaskRay</a>, <a href="https://github.com/topisani">topisani</a></td>
      <td><a href="https://github.com/jacobdufault/cquery">cquery</a></td>
      <td>C++</td>
    </tr>
    <tr>
      <td>C/C++/Objective-C</td>
      <td><a href="https://github.com/MaskRay">MaskRay</a></td>
      <td><a href="https://github.com/MaskRay/ccls">ccls</a></td>
      <td>C++</td>
    </tr>
    <tr>
      <td>CSS/LESS/SASS</td>
      <td>MS</td>
      <td><a href="https://github.com/Microsoft/vscode/tree/master/extensions/css-language-features/server">vscode-css-languageserver</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="https://ceylon-lang.org/">Ceylon</a></td>
      <td><a href="https://github.com/jvasileff">John Vasileff</a></td>
      <td><a href="https://github.com/jvasileff/vscode-ceylon">vscode-ceylon</a></td>
      <td>Ceylon</td>
    </tr>
    <tr>
      <td><a href="https://clojure.org/">√ Clojure</a></td>
      <td><a href="https://github.com/snoe">Case Nelson</a></td>
      <td><a href="https://github.com/snoe/clojure-lsp">clojure-lsp</a></td>
      <td>Clojure</td>
    </tr>
    <tr>
      <td>IBM Enterprise <strong>COBOL</strong> for z/OS</td>
      <td><a href="https://www.broadcom.com/">Broadcom</a></td>
      <td><a href="https://github.com/eclipse/che-che4z-lsp-for-cobol">COBOL Language Support</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td><a href="https://www.ibm.com/support/knowledgecenter/SS6SG3_6.3.0/welcome.html">IBM Enterprise <strong>COBOL</strong> for z/OS</a></td>
      <td>IBM</td>
      <td><a href="https://marketplace.visualstudio.com/items?itemName=IBM.zopeneditor">IBM Z Open Editor VS Code extension</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td><a href="https://securitylab.github.com/tools/codeql">CodeQL</a></td>
      <td><a href="https://github.com/github/codeql">GitHub</a></td>
      <td><a href="https://github.com/github/codeql">codeql</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td><a href="https://www.commonwl.org/">CWL</a></td>
      <td><a href="https://github.com/rabix">Rabix</a></td>
      <td><a href="https://github.com/rabix/benten">Benten</a></td>
      <td>Python</td>
    </tr>
    <tr>
      <td><a href="https://crystal-lang.org/">Crystal</a></td>
      <td><a href="https://github.com/elbywan">Elbaz Julien</a></td>
      <td><a href="https://github.com/elbywan/crystalline">Crystalline</a></td>
      <td>Crystal</td>
    </tr>
    <tr>
      <td><a href="https://crystal-lang.org/">Crystal</a></td>
      <td><a href="https://github.com/kofno">Ryan L. Bell</a></td>
      <td><a href="https://github.com/crystal-lang-tools/scry">Scry</a></td>
      <td>Crystal</td>
    </tr>
    <tr>
      <td><a href="https://dlang.org">D</a></td>
      <td><a href="https://github.com/WebFreak001">Jan Jurzitza</a></td>
      <td><a href="https://github.com/Pure-D/serve-d">serve-d</a></td>
      <td>D</td>
    </tr>
    <tr>
      <td><a href="https://dlang.org">D</a></td>
      <td><a href="https://github.com/LaurentTreguier">Laurent Tréguier</a></td>
      <td><a href="https://github.com/d-language-server/dls">D Language Server</a></td>
      <td>D</td>
    </tr>
    <tr>
      <td>Dart</td>
      <td><a href="https://dart.dev/">Dart Team</a></td>
      <td><a href="https://github.com/dart-lang/sdk/blob/master/pkg/analysis_server/tool/lsp_spec/README.md">Dart SDK</a></td>
      <td>Dart</td>
    </tr>
    <tr>
      <td><a href="https://minecraft.gamepedia.com/Data_Pack">Data Pack</a></td>
      <td><a href="https://github.com/SPGoding">SPGoding</a></td>
      <td><a href="https://github.com/SPGoding/datapack-language-server">Data-pack Language Server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="https://www.embarcadero.com/products/delphi">Delphi</a></td>
      <td><a href="https://www.embarcadero.com/">Embarcadero</a></td>
      <td>Proprietary (<a href="http://docwiki.embarcadero.com/RADStudio/en/Code_Insight_References">DelphiLSP</a>)</td>
      <td>Delphi and C</td>
    </tr>
    <tr>
      <td><a href="https://deno.land">Deno</a> (TypeScript/JavaScript)</td>
      <td><a href="https://github.com/denoland">Deno Land</a></td>
      <td><a href="https://github.com/denoland/deno/tree/main/cli/lsp"><code class="language-plaintext highlighter-rouge">deno lsp</code></a></td>
      <td>Rust</td>
    </tr>
    <tr>
      <td><a href="https://docs.docker.com/engine/reference/builder/">Dockerfiles</a></td>
      <td><a href="https://github.com/rcjsuen">Remy Suen</a></td>
      <td><a href="https://github.com/rcjsuen/dockerfile-language-server-nodejs">dockerfile-language-server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="https://secure.byond.com/">DreamMaker</a></td>
      <td><a href="https://www.platymuus.com">SpaceManiac</a></td>
      <td><a href="https://github.com/SpaceManiac/SpacemanDMM/tree/master/src/langserver">DreamMaker Language Server</a></td>
      <td>Rust</td>
    </tr>
    <tr>
      <td>Erlang</td>
      <td><a href="https://github.com/erlang">OTP</a>, <a href="https://github.com/vladdu">Vlad Dumitrescu</a></td>
      <td><a href="https://github.com/erlang/sourcer">sourcer</a></td>
      <td>Erlang</td>
    </tr>
    <tr>
      <td><a href="https://www.erlang.org">Erlang</a></td>
      <td><a href="https://github.com/robertoaloi">Roberto Aloi</a> and <a href="https://github.com/erlang-ls/erlang_ls/graphs/contributors">Contributors</a></td>
      <td><a href="https://github.com/erlang-ls/erlang_ls">erlang_ls</a></td>
      <td>Erlang</td>
    </tr>
    <tr>
      <td>Elixir</td>
      <td><a href="https://github.com/elixir-lsp">Elixir LSP</a></td>
      <td><a href="https://github.com/elixir-lsp/elixir-ls">elixir-ls</a></td>
      <td>Elixir</td>
    </tr>
    <tr>
      <td>Elm</td>
      <td><a href="https://github.com/razzeee">Kolja Lampe</a></td>
      <td><a href="https://github.com/elm-tooling/elm-language-server">elmLS</a></td>
      <td>Typescript</td>
    </tr>
    <tr>
      <td><a href="https://ember-cli.com/">Ember</a></td>
      <td><a href="https://github.com/emberwatch">Ember community</a></td>
      <td><a href="https://github.com/emberwatch/ember-language-server">ember language server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>F#</td>
      <td><a href="https://github.com/georgewfraser">@georgewfraser</a></td>
      <td><a href="https://github.com/georgewfraser/fsharp-language-server">F# Language Server</a></td>
      <td>F#</td>
    </tr>
    <tr>
      <td>F#</td>
      <td><a href="https://github.com/Krzysztof-Cieslak">@Krzysztof-Cieslak</a> &amp; Contributors</td>
      <td><a href="https://github.com/fsharp/FsAutoComplete">FsAutoComplete</a></td>
      <td>F#</td>
    </tr>
    <tr>
      <td>Fortran</td>
      <td><a href="https://github.com/hansec">Chris Hansen</a></td>
      <td><a href="https://github.com/hansec/fortran-language-server">fortran-language-server</a></td>
      <td>Python</td>
    </tr>
    <tr>
      <td>GLSL</td>
      <td><a href="https://github.com/svenstaro">Sven-Hendrik Haase (@svenstaro)</a></td>
      <td><a href="https://github.com/svenstaro/glsl-language-server">glsl-language-server</a></td>
      <td>C++</td>
    </tr>
    <tr>
      <td>Gauge</td>
      <td><a href="https://github.com/getgauge">Gauge</a></td>
      <td><a href="https://github.com/getgauge/gauge/">Gauge Language Server</a></td>
      <td>Go</td>
    </tr>
    <tr>
      <td>GDScript</td>
      <td><a href="https://github.com/godotengine">GodotEngine</a></td>
      <td><a href="https://github.com/godotengine/godot">Godot</a></td>
      <td>C++</td>
    </tr>
    <tr>
      <td>Gluon</td>
      <td><a href="https://github.com/gluon-lang/gluon">Gluon</a></td>
      <td><a href="https://github.com/gluon-lang/gluon_language-server">Gluon Language Server</a></td>
      <td>Rust</td>
    </tr>
    <tr>
      <td>Go</td>
      <td><a href="https://github.com/golang/tools">Go Team</a></td>
      <td><a href="https://github.com/golang/tools/tree/master/gopls">gopls</a></td>
      <td>Go</td>
    </tr>
    <tr>
      <td>Go</td>
      <td><a href="https://sourcegraph.com/">Sourcegraph</a></td>
      <td><a href="https://github.com/sourcegraph/go-langserver">sourcegraph-go</a></td>
      <td>Go</td>
    </tr>
    <tr>
      <td><a href="http://graphql.org/">GraphQL</a></td>
      <td><a href="https://foundation.graphql.org/">GraphQL Foundation</a></td>
      <td><a href="https://github.com/graphql/graphiql/tree/main/packages/graphql-language-service-server">Official GraphQL Language Server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="http://graphql.org/">GraphQL</a></td>
      <td><a href="https://github.com/Mayank1791989">Mayank Agarwal (@Mayank1791989)</a></td>
      <td><a href="https://github.com/Mayank1791989/gql-language-server">GQL Language Server</a></td>
      <td>JavaScript</td>
    </tr>
    <tr>
      <td>Groovy</td>
      <td><a href="https://github.com/palantir">Palantir</a></td>
      <td><a href="https://github.com/palantir/groovy-language-server/">groovy-language-server</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td><a href="http://groovy-lang.org/">Groovy</a></td>
      <td><a href="https://moonshine-ide.com/">Prominic.NET</a></td>
      <td><a href="https://github.com/prominic/groovy-language-server">Groovy Language Server</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td><a href="http://groovy-lang.org/">Groovy</a></td>
      <td><a href="https://marketplace.visualstudio.com/items?itemName=NicolasVuillamy.vscode-groovy-lint">VsCode Groovy Lint</a></td>
      <td><a href="https://github.com/nvuillam/vscode-groovy-lint/tree/master/server">VsCode Groovy Lint Language Server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>HTML</td>
      <td>MS</td>
      <td><a href="https://github.com/Microsoft/vscode/tree/master/extensions/html-language-features/server">vscode-html-languageserver</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>Haskell</td>
      <td><a href="https://github.com/alanz">Alan Zimmerman</a></td>
      <td><a href="https://github.com/haskell/haskell-language-server">Haskell Language Server (HLS)</a></td>
      <td>Haskell</td>
    </tr>
    <tr>
      <td><a href="https://haxe.org/">Haxe</a></td>
      <td><a href="https://github.com/HaxeFoundation/">Haxe Foundation</a></td>
      <td><a href="https://github.com/vshaxe/haxe-language-server">Haxe Language Server</a></td>
      <td>Haxe</td>
    </tr>
    <tr>
      <td>HLSL</td>
      <td><a href="https://github.com/tgjones">Tim Jones</a></td>
      <td><a href="https://github.com/tgjones/HlslTools/tree/master/src/ShaderTools.LanguageServer">HLSL Tools</a></td>
      <td>C#</td>
    </tr>
    <tr>
      <td><a href="https://www.cl.cam.ac.uk/research/hvg/Isabelle/">Isabelle</a></td>
      <td><a href="https://github.com/makarius">Makarius Wenzel</a></td>
      <td><a href="https://isabelle.in.tum.de/repos/isabelle/file/tip/src/Tools/VSCode">sources</a></td>
      <td>Scala</td>
    </tr>
    <tr>
      <td><a href="https://github.com/idris-lang/Idris2">Idris2</a></td>
      <td><a href="https://github.com/idris-community">Idris community</a></td>
      <td><a href="https://github.com/idris-community/idris2-lsp">idris2-lsp</a></td>
      <td>Idris2</td>
    </tr>
    <tr>
      <td>Java (Eclipse)</td>
      <td>Eclipse, <a href="http://developers.redhat.com/blog/2016/08/01/a-week-of-hacking-the-java-language-server/">RedHat</a></td>
      <td><a href="https://github.com/eclipse/eclipse.jdt.ls/">Eclipse JDT LS</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td>Java</td>
      <td><a href="https://github.com/georgewfraser">@georgewfraser</a></td>
      <td><a href="https://github.com/georgewfraser/vscode-javac">Java Compiler (javac) API-based Java support</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td>Javascript Flow</td>
      <td><a href="https://flow.org/">Facebook, Inc.</a></td>
      <td><a href="https://github.com/facebook/flow">flow</a></td>
      <td>OCaml, JavaScript</td>
    </tr>
    <tr>
      <td>Javascript Flow</td>
      <td><a href="https://github.com/flowtype/">flowtype</a></td>
      <td><a href="https://github.com/flowtype/flow-language-server">flow-language-server</a>(deprecated)</td>
      <td>JavaScript, Flow</td>
    </tr>
    <tr>
      <td>Javascript-Typescript</td>
      <td><a href="https://sourcegraph.com/">Sourcegraph</a></td>
      <td><a href="https://github.com/sourcegraph/javascript-typescript-langserver">javascript-typescript</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>JSON</td>
      <td>MS</td>
      <td><a href="https://www.npmjs.com/package/vscode-json-languageserver">vscode-json-languageserver</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="http://julialang.org/">Julia</a></td>
      <td><a href="https://github.com/JuliaEditorSupport/LanguageServer.jl/graphs/contributors">Contributors</a></td>
      <td><a href="https://github.com/JuliaEditorSupport/LanguageServer.jl">Julia language server</a></td>
      <td>Julia</td>
    </tr>
    <tr>
      <td><a href="https://ksp-kos.github.io/KOS/">Kerboscript (kOS)</a></td>
      <td><a href="https://github.com/jonnyboyC">John Chabot</a></td>
      <td><a href="https://github.com/jonnyboyC/kos-language-server">kos-language-server</a></td>
      <td>Typescript</td>
    </tr>
    <tr>
      <td><a href="https://kotlinlang.org">Kotlin</a></td>
      <td><a href="https://github.com/fwcd">@fwcd</a></td>
      <td><a href="https://github.com/fwcd/kotlin-language-server">kotlin-language-server</a></td>
      <td>Kotlin</td>
    </tr>
    <tr>
      <td><a href="https://github.com/TypeCobolTeam/LanguageServerRobot">Language Server Robot</a></td>
      <td><a href="https://github.com/TypeCobolTeam">TypeCobolTeam</a></td>
      <td><a href="https://github.com/TypeCobolTeam/LanguageServerRobot/wiki">TypeCobol Language Server Robot</a></td>
      <td>C#</td>
    </tr>
    <tr>
      <td><a href="https://languagetool.org/">LanguageTool</a></td>
      <td><a href="https://github.com/adamvoss">Adam Voss</a></td>
      <td><a href="https://github.com/adamvoss/languagetool-languageserver">languagetool</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td><a href="https://languagetool.org/">LanguageTool</a></td>
      <td><a href="https://github.com/valentjn">Julian Valentin</a></td>
      <td><a href="https://github.com/valentjn/ltex-ls">ltex-ls</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td><a href="https://www.latex-project.org/">LaTeX</a></td>
      <td><a href="https://github.com/efoerster">Eric Förster</a></td>
      <td><a href="https://github.com/efoerster/texlab">texlab</a></td>
      <td>&nbsp;</td>
    </tr>
    <tr>
      <td><a href="http://www.lua.org/">Lua</a></td>
      <td><a href="https://github.com/Alloyed">Kyle McLamb</a></td>
      <td><a href="https://github.com/Alloyed/lua-lsp">lua-lsp</a></td>
      <td>Lua</td>
    </tr>
    <tr>
      <td><a href="http://www.lua.org/">Lua</a></td>
      <td><a href="https://github.com/sumneko">最萌小汐</a></td>
      <td><a href="https://github.com/sumneko/lua-language-server">lua-language-server</a></td>
      <td>Lua</td>
    </tr>
    <tr>
      <td><a href="https://sourceforge.net/projects/lpg/">IBM <strong>LALR</strong> Parser Generator language</a></td>
      <td><a href="https://github.com/kuafuwang">kuafuwang</a></td>
      <td><a href="https://github.com/kuafuwang/LPG-language-server">LPG-language-server</a></td>
      <td>C++</td>
    </tr>
    <tr>
      <td><a href="https://github.com/Microsoft/msbuild">MSBuild</a></td>
      <td><a href="https://github.com/tintoy">Adam Friedman</a></td>
      <td><a href="https://github.com/tintoy/msbuild-project-tools-vscode">msbuild-project-tools-vscode</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>Nim</td>
      <td><a href="https://github.com/PMunch">Peter Munch-Ellingsen</a></td>
      <td><a href="https://github.com/PMunch/nimlsp">nimlsp</a></td>
      <td>Nim</td>
    </tr>
    <tr>
      <td>OCaml/Reason</td>
      <td><a href="https://github.com/freebroccolo">freebroccolo</a></td>
      <td><a href="https://github.com/freebroccolo/ocaml-language-server">ocaml-language-server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>OCaml/Reason</td>
      <td><a href="http://ocamllabs.io">OCaml Labs</a></td>
      <td><a href="https://github.com/ocaml/ocaml-lsp">ocamllsp</a></td>
      <td>OCaml</td>
    </tr>
    <tr>
      <td><a href="https://www.openapis.org/">OpenAPI</a></td>
      <td><a href="https://a.ml/">AML</a></td>
      <td><a href="https://github.com/aml-org/als">AML Language Server</a></td>
      <td>ScalaJS</td>
    </tr>
    <tr>
      <td><a href="http://openvalidation.io/">openVALIDATION</a></td>
      <td><a href="https://github.com/openvalidation">openVALIDATION</a></td>
      <td><a href="https://github.com/openvalidation/ov-language-server">ov-language-server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="https://www.creationkit.com/fallout4/index.php?title=Category:Papyrus">Papyrus</a></td>
      <td><a href="https://github.com/joelday">Joel Day</a></td>
      <td><a href="https://github.com/joelday/papyrus-lang">papyrus-lang</a></td>
      <td>C#</td>
    </tr>
    <tr>
      <td>Perl</td>
      <td>GRICHTER</td>
      <td><a href="https://metacpan.org/pod/Perl::LanguageServer">Perl::LanguageServer</a></td>
      <td>Perl</td>
    </tr>
    <tr>
      <td>Perl</td>
      <td><a href="https://github.com/FractalBoy">Marc Reisner</a></td>
      <td><a href="https://github.com/FractalBoy/perl-language-server">PLS</a></td>
      <td>Perl</td>
    </tr>
    <tr>
      <td>Crane <strong>PHP</strong></td>
      <td><a href="https://github.com/HvyIndustries">HvyIndustries</a></td>
      <td><a href="https://marketplace.visualstudio.com/items?itemName=HvyIndustries.crane">VS Code Extension</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>PHP</td>
      <td><a href="https://github.com/bmewburn">Ben Mewburn</a></td>
      <td><a href="https://github.com/bmewburn/vscode-intelephense">intelephense</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>PHP</td>
      <td><a href="https://github.com/felixfbecker">Felix Becker</a></td>
      <td><a href="https://github.com/felixfbecker/php-language-server">php-language-server</a></td>
      <td>PHP</td>
    </tr>
    <tr>
      <td>PHP</td>
      <td><a href="https://gitlab.com/Gert-dev">Tom Gerrits</a></td>
      <td><a href="https://gitlab.com/Serenata/Serenata">Serenata</a></td>
      <td>PHP</td>
    </tr>
    <tr>
      <td>PHP</td>
      <td><a href="https://github.com/TysonAndre">Tyson Andre</a></td>
      <td><a href="https://github.com/phan/phan">Phan</a></td>
      <td>PHP</td>
    </tr>
    <tr>
      <td>PHPUnit</td>
      <td><a href="https://github.com/recca0120">Recca0120</a></td>
      <td><a href="https://github.com/recca0120/vscode-phpunit">phpunit-language-server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="https://www.ibm.com/support/knowledgecenter/SSY2V3_5.3.0/com.ibm.ent.pl1.zos.doc/single_template_nobrand.html">IBM Enterprise <strong>PL/I</strong> for z/OS</a></td>
      <td>IBM</td>
      <td><a href="https://marketplace.visualstudio.com/items?itemName=IBM.zopeneditor">IBM Z Open Editor VS Code extension</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td><a href="https://www.polymer-project.org">Polymer</a></td>
      <td><a href="https://github.com/Polymer">Polymer Team</a></td>
      <td><a href="https://github.com/Polymer/polymer-editor-service">polymer-editor-service</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>PowerPC Assembly</td>
      <td><a href="https://github.com/OGoodness">Noah Fields</a></td>
      <td><a href="https://github.com/OGoodness/VSCode-PowerPC-Syntax">PowerPC Support</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>PowerShell</td>
      <td>MS</td>
      <td><a href="https://github.com/PowerShell/PowerShellEditorServices">PowerShell Editor Services</a></td>
      <td>C#</td>
    </tr>
    <tr>
      <td><a href="https://prometheus.io/docs/prometheus/latest/querying/basics/">PromQL</a></td>
      <td><a href="https://github.com/slrtbtfs">Tobias Guggenmos</a></td>
      <td><a href="https://github.com/prometheus-community/promql-langserver">promql-langserver</a></td>
      <td><a href="https://golang.org/">Go</a></td>
    </tr>
    <tr>
      <td>PureScript</td>
      <td><a href="https://github.com/nwolverson">Nicholas Wolverson</a></td>
      <td><a href="https://github.com/nwolverson/purescript-language-server">purescript-language-server</a></td>
      <td>PureScript</td>
    </tr>
    <tr>
      <td>Puppet</td>
      <td><a href="https://github.com/lingua-pupuli">Lingua Pupuli</a></td>
      <td><a href="https://github.com/lingua-pupuli/puppet-editor-services">puppet language server</a></td>
      <td>Ruby</td>
    </tr>
    <tr>
      <td>Python</td>
      <td><a href="https://github.com/fabioz/">Fabio Zadrozny</a></td>
      <td><a href="http://www.pydev.org/vscode/index.html">PyDev on VSCode</a></td>
      <td>Java, Python</td>
    </tr>
    <tr>
      <td>Python</td>
      <td><a href="https://github.com/palantir">Palantir Technologies</a></td>
      <td><a href="https://github.com/palantir/python-language-server">python-language-server</a></td>
      <td>Python</td>
    </tr>
    <tr>
      <td>Python</td>
      <td>MS</td>
      <td><a href="https://github.com/Microsoft/python-language-server">python-language-server</a></td>
      <td>C#</td>
    </tr>
    <tr>
      <td>Q#</td>
      <td><a href="https://github.com/microsoft/qsharp-compiler/graphs/contributors">MS</a></td>
      <td><a href="https://github.com/microsoft/qsharp-compiler">Q# Language Server</a></td>
      <td>C#</td>
    </tr>
    <tr>
      <td>R</td>
      <td><a href="https://github.com/REditorSupport">REditorSupport</a></td>
      <td><a href="https://github.com/REditorSupport/languageserver">R language server</a></td>
      <td>R</td>
    </tr>
    <tr>
      <td>Racket</td>
      <td><a href="https://github.com/jeapostrophe">Jay McCarthy</a></td>
      <td><a href="https://github.com/jeapostrophe/racket-langserver">racket-langserver</a></td>
      <td>Racket</td>
    </tr>
    <tr>
      <td>RAML</td>
      <td><a href="http://raml.org/about/workgroup">RAML Workgroup</a></td>
      <td><a href="https://github.com/raml-org/raml-language-server">raml-language-server</a> Work in Progress</td>
      <td>&nbsp;</td>
    </tr>
    <tr>
      <td><a href="https://raml.org/">RAML</a></td>
      <td><a href="https://a.ml/">AML</a></td>
      <td><a href="https://github.com/aml-org/als">AML Language Server</a></td>
      <td>ScalaJS</td>
    </tr>
    <tr>
      <td>ReasonML</td>
      <td><a href="https://github.com/jaredly">Jared Forsyth</a></td>
      <td><a href="https://github.com/jaredly/reason-language-server">reason-language-server</a></td>
      <td>OCaml</td>
    </tr>
    <tr>
      <td>Red</td>
      <td><a href="https://github.com/bitbegin">bitbegin</a></td>
      <td><a href="https://github.com/bitbegin/redlangserver">redlangserver</a></td>
      <td>Red</td>
    </tr>
    <tr>
      <td><a href="https://github.com/sscit/rel">REL</a></td>
      <td><a href="https://www.github.com/sscit">Stefan Schlichthärle</a></td>
      <td><a href="https://github.com/sscit/rel">rel-ls</a></td>
      <td>C++</td>
    </tr>
    <tr>
      <td><a href="https://rescript-lang.org">ReScript</a></td>
      <td><a href="https://github.com/rescript-lang">rescript-lang</a></td>
      <td><a href="https://github.com/rescript-lang/rescript-vscode">rescript-vscode</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="https://robotframework.org/">Robot Framework</a></td>
      <td><a href="https://github.com/tomi">Tomi Turtiainen</a></td>
      <td><a href="https://github.com/tomi/vscode-rf-language-server">VSCode RF language server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="https://robotframework.org">Robot Framework</a></td>
      <td><a href="https://robocorp.com">Robocorp</a></td>
      <td><a href="https://github.com/robocorp/robotframework-lsp">robotframework-lsp</a></td>
      <td>Python</td>
    </tr>
    <tr>
      <td>Ruby</td>
      <td><a href="https://github.com/castwide">Fred Snyder</a></td>
      <td><a href="https://github.com/castwide/solargraph">solargraph</a></td>
      <td>Ruby</td>
    </tr>
    <tr>
      <td>Ruby</td>
      <td><a href="https://github.com/mtsmfm">Fumiaki MATSUSHIMA</a></td>
      <td><a href="https://github.com/mtsmfm/language_server-ruby">language_server-ruby</a></td>
      <td>Ruby</td>
    </tr>
    <tr>
      <td>Ruby</td>
      <td><a href="https://github.com/DarkDimius">Dmitry Petrashko</a></td>
      <td><a href="https://github.com/sorbet/sorbet">sorbet</a></td>
      <td>Ruby</td>
    </tr>
    <tr>
      <td>Ruby</td>
      <td><a href="https://github.com/swistak35">Rafał Łasocha</a></td>
      <td><a href="https://github.com/swistak35/orbacle">orbacle</a></td>
      <td>Ruby</td>
    </tr>
    <tr>
      <td><a href="https://www.ruby-lang.org/en/">Ruby</a></td>
      <td><a href="https://github.com/kwerle/">Kurt Werle</a></td>
      <td><a href="https://github.com/kwerle/ruby_language_server">ruby_language_server</a></td>
      <td>Ruby</td>
    </tr>
    <tr>
      <td>Rust</td>
      <td><a href="https://github.com/jonathandturner">Jonathan Turner</a></td>
      <td><a href="https://github.com/rust-lang-nursery/rls">Rust Language Server</a></td>
      <td>Rust</td>
    </tr>
    <tr>
      <td>Rust</td>
      <td><a href="https://github.com/matklad">Aleksey Kladov</a></td>
      <td><a href="https://github.com/rust-analyzer/rust-analyzer">Rust Analyzer</a></td>
      <td>Rust</td>
    </tr>
    <tr>
      <td>Scala</td>
      <td><a href="https://github.com/dragos">Iulian Dragos</a></td>
      <td><a href="https://github.com/dragos/dragos-vscode-scala">dragos-vscode-scala</a></td>
      <td>Scala</td>
    </tr>
    <tr>
      <td>Scala</td>
      <td><a href="https://github.com/scalameta">Scalameta</a></td>
      <td><a href="https://github.com/scalameta/metals">Metals</a></td>
      <td>Scala</td>
    </tr>
    <tr>
      <td><a href="https://pharo.org/">Smalltalk/Pharo</a></td>
      <td><a href="https://github.com/badetitou">Benoît Verhaeghe</a></td>
      <td><a href="https://github.com/badetitou/Pharo-LanguageServer">Pharo Language Server</a></td>
      <td>Pharo</td>
    </tr>
    <tr>
      <td>SPARQL</td>
      <td><a href="https://github.com/stardog-union">Stardog Union</a></td>
      <td><a href="https://github.com/stardog-union/stardog-language-servers/tree/master/packages/sparql-language-server">SPARQL Language Server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>SQL</td>
      <td><a href="https://github.com/lighttiger2505">Toshikazu Ohashi</a></td>
      <td><a href="https://github.com/lighttiger2505/sqls">sqls</a></td>
      <td>Go</td>
    </tr>
    <tr>
      <td>Stylable</td>
      <td><a href="https://www.wix.com">Wix.com</a></td>
      <td><a href="https://github.com/wix/stylable/tree/master/packages/language-service">stylable/language-service</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>Svelte</td>
      <td><a href="https://github.com/UnwrittenFun">UnwrittenFun</a></td>
      <td><a href="https://github.com/UnwrittenFun/svelte-language-server">svelte-language-server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>Swift</td>
      <td><a href="https://github.com/apple">Apple</a></td>
      <td><a href="https://github.com/apple/sourcekit-lsp">SourceKit-LSP</a></td>
      <td>Swift</td>
    </tr>
    <tr>
      <td>Sysl</td>
      <td><a href="https://github.com/anz-bank">ANZ Bank</a></td>
      <td><a href="https://github.com/anz-bank/sysl">Sysl LSP</a></td>
      <td>Go</td>
    </tr>
    <tr>
      <td>SystemVerilog</td>
      <td><a href="https://github.com/dalance">dalance</a></td>
      <td><a href="https://github.com/dalance/svls">svls</a></td>
      <td>Rust</td>
    </tr>
    <tr>
      <td>T-SQL</td>
      <td>MS</td>
      <td><a href="https://github.com/Microsoft/vscode-mssql/tree/dev/src/languageservice">VS Code SQL extension</a></td>
      <td>TypeScript, Binary</td>
    </tr>
    <tr>
      <td><a href="https://www.terraform.io/">Terraform</a></td>
      <td><a href="https://github.com/juliosueiras">Julio Tain Sueiras</a></td>
      <td><a href="https://github.com/juliosueiras/terraform-lsp">terraform-lsp</a></td>
      <td>Go</td>
    </tr>
    <tr>
      <td><a href="https://www.terraform.io/">Terraform</a></td>
      <td><a href="https://github.com/hashicorp">HashiCorp</a></td>
      <td><a href="https://github.com/hashicorp/terraform-ls">terraform-ls</a></td>
      <td>Go</td>
    </tr>
    <tr>
      <td><a href="https://docs.tibbo.com/taiko/">Tibbo Basic</a></td>
      <td><a href="https://tibbo.com">Tibbo</a></td>
      <td><a href="https://github.com/tibbotech/tibbo-basic">tibbo-basic</a></td>
      <td>Tibbo Basic</td>
    </tr>
    <tr>
      <td><a href="http://www.ttcn-3.org">TTCN-3</a></td>
      <td><a href="https://www.nokia.com">Nokia</a></td>
      <td><a href="https://github.com/nokia/ntt">ntt</a></td>
      <td>Go</td>
    </tr>
    <tr>
      <td>Turtle</td>
      <td><a href="https://github.com/stardog-union">Stardog Union</a></td>
      <td><a href="https://github.com/stardog-union/stardog-language-servers/tree/master/packages/turtle-language-server">Turtle Language Server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>TypeCobol</td>
      <td><a href="https://github.com/TypeCobolTeam">TypeCobol</a></td>
      <td><a href="https://github.com/TypeCobolTeam/TypeCobol/tree/master/TypeCobol.LanguageServer">TypeCobol language server</a></td>
      <td>C#</td>
    </tr>
    <tr>
      <td>TypeScript</td>
      <td><a href="http://typefox.io/">TypeFox</a></td>
      <td><a href="https://github.com/theia-ide/typescript-language-server">typescript-language-server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>Vala</td>
      <td><a href="https://github.com/benwaffle">Ben Iofel</a>, <a href="https://github.com/Prince781">Princeton Ferro</a></td>
      <td><a href="https://github.com/benwaffle/vala-language-server">vala-language-server</a></td>
      <td>Vala</td>
    </tr>
    <tr>
      <td>VDM-SL, VDM++, VDM-RT</td>
      <td><a href="https://github.com/nickbattle">Nick Battle</a></td>
      <td><a href="https://github.com/nickbattle/vdmj/tree/master/lsp">VDMJ-LSP</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td>VHDL</td>
      <td><a href="https://github.com/kraigher">Olof Kraigher</a></td>
      <td><a href="https://github.com/kraigher/rust_hdl">vhdl_ls</a></td>
      <td>Rust</td>
    </tr>
    <tr>
      <td>Viml</td>
      <td><a href="https://github.com/iamcco">iamcco</a></td>
      <td><a href="https://github.com/iamcco/vim-language-server">vim-language-server</a></td>
      <td>Typescript</td>
    </tr>
    <tr>
      <td>Visualforce</td>
      <td>Salesforce</td>
      <td><a href="https://marketplace.visualstudio.com/items?itemName=salesforce.salesforcedx-vscode-visualforce">VS Code Visualforce extension</a></td>
      <td>&nbsp;</td>
    </tr>
    <tr>
      <td>Vue</td>
      <td><a href="https://github.com/octref">Pine Wu</a> &amp; <a href="https://github.com/HerringtonDarkholme">@HerringtonDarkholme</a></td>
      <td><a href="https://github.com/vuejs/vetur">vuejs/vetur</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="https://webassembly.org/">WebAssembly</a></td>
      <td><a href="https://github.com/darinmorrison">Darin Morrison</a></td>
      <td><a href="https://github.com/wasm-lsp/wasm-language-server">wasm-language-server</a></td>
      <td>Rust</td>
    </tr>
    <tr>
      <td><a href="https://www.wolfram.com/language/">Wolfram Language</a> (<a href="https://www.wolfram.com/mathematica">Mathematica</a>)</td>
      <td><a href="https://github.com/kenkangxgwe">kenkangxgwe</a></td>
      <td><a href="https://github.com/kenkangxgwe/lsp-wl">lsp-wl</a></td>
      <td>Wolfram Language</td>
    </tr>
    <tr>
      <td>WXML</td>
      <td><a href="https://github.com/chemzqm">Qiming Zhao</a></td>
      <td><a href="https://github.com/chemzqm/wxml-languageserver">wxml-languageserver</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>XML</td>
      <td>IBM</td>
      <td><a href="https://github.com/microclimate-devops/xml-language-server">XML Language Server</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td>XML</td>
      <td><a href="https://github.com/redhat-developer">Red Hat Developers</a> and <a href="https://github.com/angelozerr/">Angelo ZERR</a></td>
      <td><a href="https://github.com/eclipse/lemminx">XML Language Server (LemMinX)</a></td>
      <td>Java</td>
    </tr>
    <tr>
      <td>YAML (with JSON schemas)</td>
      <td><a href="https://github.com/adamvoss">Adam Voss</a></td>
      <td><a href="https://github.com/adamvoss/vscode-yaml-languageservice">vscode-yaml-languageservice</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td>YAML</td>
      <td><a href="https://github.com/redhat-developer">Red Hat Developers</a></td>
      <td><a href="https://github.com/redhat-developer/yaml-language-server">yaml-language-server</a></td>
      <td>TypeScript</td>
    </tr>
    <tr>
      <td><a href="https://tools.ietf.org/html/rfc7950">YANG</a></td>
      <td><a href="https://github.com/yang-tools">Yang tools</a></td>
      <td><a href="https://github.com/yang-tools/yang-lsp">yang-lsp</a></td>
      <td>XTend</td>
    </tr>
    <tr>
      <td>Zig</td>
      <td><a href="https://github.com/orgs/zigtools">Zig Developers</a></td>
      <td><a href="https://github.com/zigtools/zls">zls</a></td>
      <td><a href="https://ziglang.org">Zig</a></td>
    </tr>
    <tr>
      <td>*</td>
      <td><a href="https://github.com/mattn">mattn</a></td>
      <td><a href="https://github.com/mattn/efm-langserver">efm-langserver</a></td>
      <td><a href="https://golang.org/">Go</a></td>
    </tr>
  </tbody>
</table>
</div>
        
