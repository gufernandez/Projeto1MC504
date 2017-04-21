#[Trabalho 1 MC504] (http://www.ic.unicamp.br/~islene/1s2017-mc504/proj1.html)

*O Chamys fez esse belo README no trabalho do Borin e estou copiando aqui, porque é muito útil pra lembrar coisas do git que eu esqueço (Algumas coisas estavam erradas, mas nada demais e não lembro onde)*

Um mini-tutorial está em baixo, mas tem um guia básico que parece interessante de se ler: [Git - The Simple Guide] (http://rogerdudler.github.io/git-guide/).

Um outro blog legal de se ler é o [blog do Git](https://git-scm.com/blog).

##Começar a trabalhar:

Legenda: *recomendável*, `comando`.

1. Inicializar o git : `$ git init`
2. *Criar uma pasta git caso não tenha*
3. Realizar o pull request: `$ git pull https://github.com/gufernandez/Projeto1MC504`
4. Clonar: `$ git clone https://github.com/gufernandez/Projeto1MC504`
5. Ir até a pasta e criar um branch pra você: `$ git checkout -b Eu`
6. Dar push no branch: `$ git push origin Eu`
7. Codar hardmente

##Comandos

###Coisas básicas
 - Adicionar arquivos no repositório: `$ git add <arquivos>`
 - Registrar suas mudanças: `$ git commit -m "Sua mensagem aqui"`
 - Fazer upload: `$ git push origin <branch>` (De preferência, no seu branch)
 - Verificar status: `$ git status`

##Lidar com branches
 - Ver os branches existentes e em qual está: `$ git branch`
 - Criar novo branch: `$ git branch <nome>`
 - Mudar de branch: `$ git checkout <nome>`

###Atualizar o arquivo compartilhado
Mesmo se você não quiser colocar os seus arquivos no git, tem de usar o seu branch para gerenciar melhor a mudança do arquivo principal.

Achei uma solução bem simples pro nosso problema: [How to merge changes to a single file](http://stackoverflow.com/questions/10784523/how-do-i-merge-changes-to-a-single-file-rather-than-merging-commits)

Basicamente,você está no branch *Eu* e mudou certo arquivo que precisa ser atualizado no master. Faça o seguinte:

 1. Mude para o branch *master*: `$ git checkout master`
 2. Una os dois arquivos: `$ git checkout --patch Eu <arquivo>`
 3. Gerencie as mudanças (pode aceitar parte por parte com 'y' ou tudo com 'a' ou editar o arquivo final com 'e')
 4. Atualize a tag: `$ git tag -a vx.y.z -m "Mudança"`

    >Sendo: x = 0 ou 1 (projeto completo ou não), y += 1 a cada função adicionada e z += 1 quando bugs são corrigidos

 5. Registre as mudanças com commit: `$ git commit -m "Texto aqui"`
 6. Dê o upload dos arquivos: `$ git push`
 7. Dê o upload das tags: `$ git push --tags`

##Tarefas:
- [X] Copiar README do Chamys
- [ ] Fazer base do programa
- [ ] Fazer animação
