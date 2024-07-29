# Pawn Community Compiler

[![Status de Construção][build_status_linux]][build_linux]
[![Status de Construção - Windows][build_status_win]][build_win]
## Novidades
 Esta Versao foi traduzida para Portugues 
    Em Breve mais novidades
## O que é

Esta é uma versão modificada do compilador Pawn 3.2.3664 com muitas correções de bugs e melhorias.

Este projeto foi originalmente iniciado por Zeex, mas em 31 de dezembro de 2017, o projeto foi assumido por alguns membros da comunidade SA:MP. Zeex ainda contribui para o projeto, junto com a [Equipe do Compilador][team].

O README original está disponível [aqui][original_readme]

## Por que

Este projeto existe para:

- Corrigir bugs conhecidos no compilador original
- Proporcionar uma melhor experiência de desenvolvimento para a comunidade SA:MP

Se você encontrar um problema, pode [abrir uma issue][issues] e os contribuintes podem trabalhar em uma solução. Isso não é possível com o compilador original que vem com a distribuição do servidor SA:MP.

Além disso, há novos recursos que melhoram a experiência de desenvolvimento, como informar quais tags estão envolvidas em um "desajuste de tags" ou mostrar onde aquele "símbolo nunca é usado" está realmente declarado.

Há muitos recursos e correções documentados; veja abaixo os links:

- [Bugs conhecidos do compilador][bugs] contém uma lista de bugs que a equipe está ciente, com seu status.

- [Novidades][new] contém recursos e outras mudanças notáveis.

- [Notas de lançamento][releases] lista de todos os lançamentos oficiais dos binários do compilador.

## Como Usar

Pacotes binários podem ser baixados em [Releases][releases]; veja as seções abaixo para instruções específicas de instalação para cada plataforma.

**Nota:** Você _provavelmente_ receberá avisos/erros/fatais quando construir pela primeira vez com este compilador — você precisará adicionar a flag `-Z` à sua configuração de compilação ou adicionar `#pragma compat`. Veja [esta página][compat] para mais informações.

Você também pode obter os binários de desenvolvimento mais recentes para Windows em [AppVeyor][artifacts]. Este arquivo é construído automaticamente a cada commit no Git e pode ser bastante instável, então use por sua conta e risco.

### Windows

Se você usa um editor configurado para executar `pawncc`, como Pawno, Sublime Text ou VS Code, você pode simplesmente deletar seu `pawncc.exe` existente e substituí-lo pelo novo.

Baixe o arquivo ZIP e extraia `pawnc.dll`, `pawncc.exe`, `pawndisasmsm.exe` para o diretório original do `pawncc`. Se você usar o Pawno, isso estará na pasta `pawno/` que está dentro do seu diretório do servidor.

### openSUSE

Há um pacote de instalação disponível para usuários do openSUSE, permitindo que você instale facilmente a versão mais recente do compilador em sua distribuição. Siga estes passos:

1. Vá para https://build.opensuse.org/package/show/home:mschnitzer:pawncc/pawncc
2. No lado direito, selecione sua distribuição (somente se não estiver desativada!)
3. Clique em "Ir para repositório de download"
4. Copie o link e entre em modo sudo no seu terminal:
    `zypper ar $COPIED_LINK home:mschnitzer:pawncc`
5. Novamente como root, digite: `zypper ref`
6. Instale o pacote com `zypper in pawncc`
7. Execute `pawncc` no seu terminal para testar se está funcionando

O repositório de download para openSUSE também oferece versões mais antigas (a versão mais antiga: 3.10.7). Basta instalar a versão que você preferir (por exemplo: `zypper in pawncc-3.10.8` e execute via `$ pawncc-3.10.8 -Z -O3 [...]`). A instalação paralela é suportada.

### Com sampctl

Se você é um usuário do [sampctl][sampctl], você já está usando este compilador!

### Compilando a partir do Código-Fonte

Se você está interessado em contribuir, primeiro leia [este documento][contributing] e certifique-se de ter discutido suas mudanças propostas antes de escrever qualquer código. Confira [esta página][build_source] para instruções sobre como compilar para sua plataforma.

## Histórico

O projeto foi originalmente iniciado como um conjunto de patches com o objetivo de criar um compilador que fosse compatível com o compilador usado no [SA-MP (San Andreas Multiplayer)](http://sa-mp.com/).

O SA-MP usa uma versão modificada do Pawn 3.2.3664 [1] com executáveis exclusivos para Windows, e os desenvolvedores disseram que perderam o código-fonte, o que significa que não pode ser portado para outras plataformas (por exemplo, Linux) e bugs recentemente descobertos não podem ser corrigidos. Portanto, o objetivo principal do projeto é recriar mudanças que foram feitas anteriormente pelos desenvolvedores, bem como corrigir todos os bugs conhecidos do compilador.

[1] Vale a pena notar que a versão do AMX incorporada ao servidor SA-MP parece ser baseada em uma versão mais antiga do Pawn.

[build_linux]: https://travis-ci.org/pawn-lang/compiler
[build_status_linux]: https://travis-ci.org/pawn-lang/compiler.svg?branch=master
[build_win]: https://ci.appveyor.com/project/Southclaws/compiler/branch/master
[build_status_win]:
  https://ci.appveyor.com/api/projects/status/k112tbr1afrkif0n?svg=true
[team]: https://github.com/pawn-lang/compiler/graphs/contributors
[original_readme]:
  https://github.com/pawn-lang/compiler/tree/master/readme_compuphase.txt
[issues]: https://github.com/pawn-lang/compiler/issues
[bugs]: https://github.com/pawn-lang/compiler/wiki/Known-compiler-bugs
[new]: https://github.com/pawn-lang/compiler/wiki/What's-new
[releases]: https://github.com/pawn-lang/compiler/releases
[artifacts]:
  https://ci.appveyor.com/project/Southclaws/compiler/branch/master/artifacts
[compat]: https://github.com/pawn-lang/compiler/wiki/Compatibility-mode
[sampctl]: http://bit.ly/sampctl
[contributing]:
  https://github.com/pawn-lang/compiler/tree/master/.github/CONTRIBUTING.md
[build_source]: https://github.com/pawn-lang/compiler/wiki/Building-From-Source
